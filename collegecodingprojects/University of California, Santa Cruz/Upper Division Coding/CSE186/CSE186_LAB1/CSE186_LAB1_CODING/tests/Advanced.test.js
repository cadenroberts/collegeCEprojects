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
 * Setup for advanced tests
 */
beforeAll(async () => {
  fs.copyFileSync("src/index.html", "src/tester.html"); 
  replace({
    regex: '<head>(?:.|\n|\r)+?<\/head>',
    replacement: `<head> <link href="advanced.css" rel="stylesheet" /></head`,
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
  await screenshots.compare(page, 'advanced-static');
});

/**
 * Letter C Hover
 */
test('letter C hover', async () => {
  await page.goto('file://' + process.cwd() + '/src/tester.html');
  await page.hover("text/C");
  await screenshots.compare(page, 'advanced-letter-C-hover');
});

/**
 * Letter S Hover
 */
test('letter S hover', async () => {
  await page.goto('file://' + process.cwd() + '/src/tester.html');
  await page.hover("text/S");
  await screenshots.compare(page, 'advanced-letter-S-hover');
});

/**
 * Letter E Hover
 */
test('letter E hover', async () => {
  await page.goto('file://' + process.cwd() + '/src/tester.html');
  await page.hover("text/E");
  await screenshots.compare(page, 'advanced-letter-E-hover');
});

/**
 * Number 1 Hover
 */
test('number 1 hover', async () => {
  await page.goto('file://' + process.cwd() + '/src/tester.html');
  await page.hover("text/1");
  await screenshots.compare(page, 'advanced-number-1-hover');
});

/**
 * Number 8 Hover
 */
test('number 8 hover', async () => {
  await page.goto('file://' + process.cwd() + '/src/tester.html');
  await page.hover("text/8");
  await screenshots.compare(page, 'advanced-number-8-hover');
})

/**
 * Number 6 Hover
 */
test('number 6 hover', async () => {
  await page.goto('file://' + process.cwd() + '/src/index.html');
  await page.hover("text/6");
  await screenshots.compare(page, 'advanced-number-6-hover');
})
