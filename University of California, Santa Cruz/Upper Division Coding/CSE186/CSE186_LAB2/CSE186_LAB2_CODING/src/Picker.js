/**
 * CSE186 Assignment 2 - Advanced
 */

class Picker {
  /**
   * Create a date picker
   * @param {string} containerId id of a node the Picker will be a child of
   */
// code has been heavily condensed with inline function definitions and variable reduction
  constructor(containerId) {
    this.currentDate = new Date();
    this.container = document.getElementById(containerId);
    this.container.innerHTML = `<div id="datePicker"><div id="controls"><button id="prev">&lt;</button><div id="display"></div><button id="next">&gt;</button></div><div id="calendar"><div class="daysWeek"><span>Sun</span><span>Mon</span><span>Tue</span><span>Wed</span><span>Thu</span><span>Fri</span><span>Sat</span></div><div class="daysMonth">${Array.from({ length: 42 }, (_, i) => `<div id="d${i}"></div>`).join('')}</div></div></div>`;
    this.container.querySelector('#prev').addEventListener('click', () => {
      this.currentDate.setMonth(this.currentDate.getMonth() - 1);
      this.updateDisplay();
    });
    this.container.querySelector('#next').addEventListener('click', () => {
      this.currentDate.setMonth(this.currentDate.getMonth() + 1);
      this.updateDisplay();
    });
    this.container.querySelector('#display').addEventListener('click', () => {
      this.currentDate = new Date();
      this.updateDisplay();
    });
    this.container.querySelector('.daysMonth').addEventListener('click', () => {
      if (event.target.matches('.daysMonth div')) {
        let newDate = new Date(this.currentDate);
        if (event.target.classList.contains('greyed')) {
          newDate.setMonth(newDate.getMonth() + 1);
          if (parseInt(event.target.textContent, 10)>15) newDate.setMonth(newDate.getMonth() - 2);
        }
        newDate.setDate(parseInt(event.target.textContent, 10));
        this.currentDate = newDate;
        this.updateDisplay();
      }
    });
    this.updateDisplay();
  }

  updateDisplay() {
    this.container.querySelector('#display').textContent = new Intl.DateTimeFormat('en-US', { year: 'numeric', month: 'long', day: 'numeric' }).format(this.currentDate);
    const days = [];
    for (let i = (new Date(this.currentDate.getFullYear(), this.currentDate.getMonth(), 1).getDay()); i > 0; i--) {
      days.push({
        date: new Date(this.currentDate.getFullYear(), this.currentDate.getMonth() - 1, (new Date(this.currentDate.getFullYear(), this.currentDate.getMonth(), 0).getDate()) - i + 1),
        greyed: true
      });
    }
    for (let i = 1; i <= (new Date(this.currentDate.getFullYear(), this.currentDate.getMonth() + 1, 0).getDate()); i++) {
      days.push({
        date: new Date(this.currentDate.getFullYear(), this.currentDate.getMonth(), i),
        greyed: false
      });
    }
    for (let i = 1; days.length < 42; i++) {
      days.push({
        date: new Date(this.currentDate.getFullYear(), this.currentDate.getMonth() + 1, i),
        greyed: true
      });
    }
    this.container.querySelector('.daysMonth').innerHTML = days.map((day, index) => { return `<div id="d${index}" class="${day.greyed ? 'greyed' : ''} ${(day.date.getDate() === this.currentDate.getDate() && day.date.getMonth() === this.currentDate.getMonth() && day.date.getFullYear() === this.currentDate.getFullYear()) ? 'highlighted' : ''}">${day.date.getDate()}</div>`; }).join('');
  }
}

document.addEventListener('DOMContentLoaded', () => {
  new Picker('picker');
});
