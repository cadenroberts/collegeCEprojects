/*
#######################################################################
#
# Copyright (C) 2020-2024 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without
# the express written permission of the copyright holder.
#
#######################################################################
*/
import {it, expect} from 'vitest';
import {render, screen, fireEvent} from '@testing-library/react';
import App from '../App';

it('check the calender and cover picker', () => {
  const {container} = render(<App />);
  const t = new Date();
  expect(screen.getByLabelText('Go to Previous Month')).toBeInTheDocument();
  expect(screen.getByLabelText('Go to Next Month')).toBeInTheDocument();
  expect(screen.getByLabelText('Return to Selected Date')).toBeInTheDocument();
  expect(screen.getByText('Sun')).toBeInTheDocument();
  expect(screen.getByText('Mon')).toBeInTheDocument();
  expect(screen.getByText('Tue')).toBeInTheDocument();
  expect(screen.getByText('Wed')).toBeInTheDocument();
  expect(screen.getByText('Thu')).toBeInTheDocument();
  expect(screen.getByText('Fri')).toBeInTheDocument();
  expect(screen.getByText('Sat')).toBeInTheDocument();
  expect(screen.getByLabelText('Enter date as MM/DD/YYYY')).toBeInTheDocument();
  expect(screen.getByLabelText('setDate')).toBeInTheDocument();
  expect(screen.getByText(`${[
    'January', 'February', 'March', 'April', 'May', 'June',
    'July', 'August', 'September', 'October', 'November', 'December',
  ][t.getMonth()]} ${t.getFullYear()}`)).toBeInTheDocument();
  expect(screen.getByLabelText(
      `${`${[
        'January', 'February', 'March', 'April', 'May', 'June',
        'July', 'August', 'September', 'October', 'November', 'December',
      ][t.getMonth()]} ${t.getDay()} ${t.getFullYear()}`} (selected)`,
  )).toHaveTextContent(t.getDate().toString());
  for (let i = (new Date(t.getFullYear(), t.getMonth(), 1)).getDay();
    i > 0; i--) {
    const prevMonthDay = new Date( t.getFullYear(), t.getMonth(),
        -i + 1).getDate();
    expect(screen.getAllByText(prevMonthDay.toString()).length,
    ).toBeGreaterThan(0);
    screen.getAllByText(prevMonthDay.toString()).forEach((e) => {
      expect(e).toBeInTheDocument();
    });
  }
  for (let i = 1; i < 7 - (new Date(t.getFullYear(),
      t.getMonth() + 1, 0)).getDay(); i++) {
    expect(screen.getAllByText(i.toString()).length).toBeGreaterThan(0);
    screen.getAllByText(i.toString()).forEach((e) => {
      expect(e).toBeInTheDocument();
    });
  }
  fireEvent.click(screen.getByLabelText('Go to Next Month'));
  screen.queryAllByLabelText(/\d{1,2} \w+ \d{4}/).forEach((label) => {
    expect(label.getAttribute('aria-label')).not.toContain('(selected)');
  });
  fireEvent.click(screen.getByLabelText('Go to Previous Month'));
  expect(screen.getByLabelText(
      `${`${[
        'January', 'February', 'March', 'April', 'May', 'June',
        'July', 'August', 'September', 'October', 'November', 'December',
      ][t.getMonth()]} ${t.getDay()} ${t.getFullYear()}`} (selected)`,
  )).toBeInTheDocument();
  fireEvent.click(screen.getByLabelText('Go to Previous Month'));
  screen.queryAllByLabelText(/\d{1,2} \w+ \d{4}/).forEach((label) => {
    expect(label.getAttribute('aria-label')).not.toContain('(selected)');
  });
  fireEvent.click(container.querySelector('.greyed'));
  fireEvent.click(screen.getByLabelText('Return to Selected Date'));
});
