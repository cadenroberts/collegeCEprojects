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
import React from 'react';

import './Picker.css';

/**
 * Picker
 */
class Picker extends React.Component {
  state = {
    reset: new Date(),
    currentDate: new Date(),
    displayDate: new Date(),
    days: [],
  };

  /**
   * Called immediately after the component is mounted.
   */
  componentDidMount() {
    this.updateDisplay();
  }

  /**
   * Called immediately after updating occurs.
   * @param {object} prevProps - The previous props.
   * @param {object} prevState - The previous state.
   */
  componentDidUpdate(prevProps, prevState) {
    if (prevState.currentDate !== this.state.currentDate ||
      prevState.displayDate !== this.state.displayDate ||
      prevState.reset !== this.state.reset) {
      this.updateDisplay();
    }
  }

  /**
   * Updates the display of days in the calendar.
   */
  updateDisplay = () => {
    const {displayDate} = this.state;
    const days = [];
    for (let i = new Date(displayDate.getFullYear(),
        displayDate.getMonth(), 1).getDay(); i > 0; i--) {
      days.push({
        date: new Date( displayDate.getFullYear(), displayDate.getMonth() - 1,
            new Date(displayDate.getFullYear(),
                displayDate.getMonth(), 0).getDate() - i + 1),
        greyed: true,
      });
    }
    for (let i = 1; i <= new Date(displayDate.getFullYear(),
        displayDate.getMonth() + 1, 0).getDate(); i++) {
      days.push({
        date: new Date(displayDate.getFullYear(), displayDate.getMonth(), i),
        greyed: false,
      });
    }
    for (let i = 1; days.length < 42; i++) {
      days.push({
        date: new Date(displayDate.getFullYear(), displayDate.getMonth() + 1,
            i),
        greyed: true,
      });
    }
    this.setState({days});
  };

  /**
   * Renders the picker component.
   * @return {object} The rendered component.
   */
  render() {
    const {currentDate, displayDate, days} = this.state;
    return (
      <div aria-label="datePicker">
        <div aria-label="controls">
          <button aria-label="Go to Previous Month"
            onClick={() => this.setState((state) => ({
              displayDate: new Date(state.displayDate.setMonth(
                  state.displayDate.getMonth() - 1))}))}>
            &lt;
          </button>
          <div aria-label="Return to Selected Date"
            onClick={() => this.setState((state) => ({
              currentDate: new Date(this.state.reset),
              displayDate: new Date(this.state.reset)}))}>
            {new Intl.DateTimeFormat('en-US', {
              year: 'numeric',
              month: 'long',
            }).format(displayDate)}
          </div>
          <button aria-label="Go to Next Month"
            onClick={() => this.setState((state) => ({
              displayDate: new Date(state.displayDate.setMonth(
                  state.displayDate.getMonth() + 1))}))}>
            &gt;
          </button>
        </div>
        <div aria-label="calendar">
          <div className="daysWeek">
            <span>Sun</span>
            <span>Mon</span>
            <span>Tue</span>
            <span>Wed</span>
            <span>Thu</span>
            <span>Fri</span>
            <span>Sat</span>
          </div>
          <div className="daysMonth">
            {days && days.map((day, index) => (
              <div key={index}
                aria-label={`${new Intl.DateTimeFormat('en-US', {
                  year: 'numeric', month: 'long', day: 'numeric',
                }).format(day.date).replace(',', '')}${
                    day.date.getDate() === displayDate.getDate() &&
                    day.date.getMonth() === displayDate.getMonth() &&
                    day.date.getFullYear() === displayDate.getFullYear() ?
                    ' (selected)' : ''
                }`}
                className={`day${day.greyed ? ' greyed' :
                (!day.greyed && day.date.getDate() === currentDate.getDate() &&
                day.date.getMonth() === currentDate.getMonth() &&
                day.date.getFullYear() === currentDate.getFullYear() ?
                ' selected' : '')}`}
                onClick={() => {
                  const newDate = new Date(day.date.getFullYear(),
                      day.date.getMonth(), day.date.getDate());
                  if (day.greyed) {
                    newDate.setMonth(newDate.getMonth() + 1);
                    if (newDate.getDate() > 15) {
                      newDate.setMonth(newDate.getMonth() - 2);
                    }
                  }
                  this.setState({reset: new Date(currentDate),
                    currentDate: newDate,
                    displayDate: newDate});
                }}
              >
                {day.date.getDate()}
              </div>
            ))}
          </div>
        </div>
      </div>
    );
  }
}

export default Picker;
