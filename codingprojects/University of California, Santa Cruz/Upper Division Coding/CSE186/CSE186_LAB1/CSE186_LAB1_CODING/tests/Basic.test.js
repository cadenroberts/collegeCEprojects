/*
 * Copyright (C) 2024 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */
/*
 * *****************************************************
 *              DO NOT MODIFY THIS FILE
 * *****************************************************
 */
const puppeteer = require('puppeteer');
const fs = require('fs');
const replace = require("replace");

const screenshots = require('./screenshots');

let browser;
let page;

/**
 * Setup for basic tests
 */
beforeAll(async () => {
  fs.copyFileSync("src/index.html", "src/tester.html"); 
  replace({
    regex: '<head>(?:.|\n|\r)+?<\/head>',
    replacement: `<head> <link href="basic.css" rel="stylesheet" /></head`,
    paths: ['src/tester.html'],
    silent: true,
  });
});

/**
 * Tear down
 */
afterAll(async () => {
  fs.unlinkSync('src/tester.html')
});

/**
 * Create a headless (not visible) instance of Chrome for each test
 * and open a new page (tab).
 */
beforeEach(async () => {
  browser = await puppeteer.launch({
    headless: true,
    // headless: false,
    // slowMo: 100,
  });
  page = await browser.newPage();
  return page.setViewport({width: 800, height: 600});
});

/**
 * Close the headless instance of Chrome as we no longer need it.
 */
afterEach(async () => {
  await browser.close();
});

/**
 * Static rendering must match known screenshot
 */
test('static', async () => {
  await page.goto('file://' + process.cwd() + '/src/tester.html');
  await screenshots.compare(page, 'basic-static');
});
