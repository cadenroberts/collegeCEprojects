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

/*
#######################################################################
# DO NOT MODIFY THIS SECTION
#######################################################################
*/

const puppeteer = require('puppeteer');
const pti = require('puppeteer-to-istanbul');
const fs = require('fs');

let browser;
let page;

beforeAll(async () => {
  browser = await puppeteer.launch({
    headless: 'new',
    args: [
      '--no-sandbox',
    ],
  });
  page = await browser.newPage();
    
//
    page.on('console', msg => {
      for (let i = 0; i < msg.args().length; ++i) {
        console.log(`${msg.args()[i]}`);
      }
    });
    
//
  await Promise.all([
    page.coverage.startJSCoverage({resetOnNavigation: false}),
    page.coverage.startCSSCoverage(),
  ]);
});

afterAll(async () => {
  const [jsCoverage] = await Promise.all([
    page.coverage.stopJSCoverage(),
  ]);
  const coverage = [];
  for (const result of jsCoverage) {
    if (result.url.endsWith('.js')) {
      coverage.push(result);
    }
  }
  pti.write([...coverage],
      {includeHostname: true, storagePath: './.nyc_output'});
  fs.renameSync('./.nyc_output/out.json', './.nyc_output/picker.json');
  await browser.close();
});

beforeEach(async () => {
  await page.goto(`file://${__dirname}/../src/picker.html`);
});


/**
 * @param {string} id
 */
async function exists(id) {
  const element = await page.$(`#${id}`);
  expect(element).not.toBeNull();
}

test('Picker Exists', async () => {
  await exists('picker');
});

/*
#######################################################################
# END DO NOT MODIFY SECTION
#######################################################################
*/

/*
#######################################################################
# Add your tests for the Advanced requirement below here.
# Do not create new page instances, use the one defined at line 23 and
# assigned at line 32.
#######################################################################
*/

/*
#########################################################################
# Here's a test and a helper function to get you started, uncomment them
# when you're ready
#########################################################################
*/

// /**
//  * @param {number} relative months ahead or behind the current date
//  * @return {date} first day of the relative month
//  */
 function firstDay(relative) {
   const date = new Date();
   date.setDate(1);
   date.setMonth(date.getMonth()+relative);
   return date.getDay();
 }

// Clicks the next button on picker a random number of times then checks
// the first day of the displayed month is correct.
// For example, July 1, 2021 falls on a Thursday, the 5th day of the week
// when weeks start on Sundays.

// To achieve full coverage, the actual success checks are not needed, however, I have included the corresponding checks, which all pass with my code. (each heavily condensed into one line). I was having trouble acheiving full coverage and used chat GPT along with an EXTENSIVE amount of my own edits and condensing to achieve full coverage.
 test('Navigate Full Coverage', async () => {
   await page.click('#next'); // next click
   expect((await (await (await page.$('#d'+firstDay(1))).getProperty('textContent')).jsonValue()).trim()).toBe('1'); // next test
   await page.click('#display'); // reset click
   expect(await (await (await page.$('#display')).getProperty('textContent')).jsonValue()).toContain((new Date()).toLocaleString('default', { month: 'long', day: '2-digit', year: 'numeric' })); // reset test
   await page.click('#prev'); // prev click
   expect((await (await (await page.$('#d' + firstDay(-1))).getProperty('textContent')).jsonValue()).trim()).toBe('1'); // prev test
   expect(await page.evaluate(() => { return document.querySelector('#display').textContent; })).not.toBe(await page.evaluate(() => { document.querySelectorAll('.daysMonth div.greyed')[0].click(); /* grey click */ return document.querySelector('#display').textContent; })); // grey test
 });
 











