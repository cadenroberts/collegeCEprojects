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
import {it, expect, vi} from 'vitest';
import {render, screen, fireEvent} from '@testing-library/react';
import Entry from '../Entry';

it('check input and button click and cover entry', () => {
  const pickerRef = {current: {setState: vi.fn()}};
  render(<Entry pickerRef={pickerRef} />);
  // Simulate entering a valid date
  fireEvent.change(screen.getByLabelText(
      'Enter date as MM/DD/YYYY'), {target: {value: '2024-12-25'}});
  expect(screen.getByLabelText(
      'Enter date as MM/DD/YYYY').value).toBe('2024-12-25');
  // Simulate clicking the set date button
  fireEvent.click(screen.getByLabelText('setDate'));
  expect(pickerRef.current.setState).toHaveBeenCalledWith({
    currentDate: new Date(2024, 11, 25),
    displayDate: new Date(2024, 11, 25),
  });
});
