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
#######               DO NOT MODIFY THIS FILE               ###########
#######################################################################
*/

const puppeteer = require('puppeteer');
const http = require('http');
const path = require('path');
const express = require('express');

require('dotenv').config();
const app = require('../../backend/src/app');

let backend;
let frontend;
let browser;
let page;

beforeAll(() => {
  backend = http.createServer(app);
  backend.listen(3010, () => {
    console.log('Backend Running at http://localhost:3010');
  });
  frontend = http.createServer(
    express()
      .use('/assets', express.static(
        path.join(__dirname, '..', '..', 'frontend', 'dist', 'assets')))
      .get('*', function(req, res) {
        res.sendFile('index.html',
          {root: path.join(__dirname, '..', '..', 'frontend', 'dist')});
      }),
  );
  frontend.listen(3000, () => {
    console.log('Frontend Running at http://localhost:3000');
  });
});

afterAll(async () => {
  await backend.close();
  await frontend.close();
  setImmediate(function(){frontend.emit('close')});
});

beforeEach(async () => {
  browser = await puppeteer.launch({
    headless: true,
    // Use these settings if you want to see the browser
    // headless: false,
    // slowMo: 100,
  });
  page = await browser.newPage();
  await page.goto('http://localhost:3000');
});

afterEach(async () => {
  const childProcess = browser.process();
  if (childProcess) {
    await childProcess.kill(9);
  }
});

test('Initial View', async () => {
  const label = await page.waitForSelector(
    '::-p-text(CSE186 Full Stack Mail - Inbox)');
  expect(label).not.toBeNull();
});

test('Trash Selection', async () => {
  await page.click('::-p-aria(Select Trash)');
  const label = await page.waitForSelector(
    '::-p-text(CSE186 Full Stack Mail - Trash)');
  expect(label).not.toBeNull();
});

test('Sent Selection', async () => {
  await page.click('::-p-aria(Select Sent)');
  const label = await page.waitForSelector(
    '::-p-text(CSE186 Full Stack Mail - Sent)');
  expect(label).not.toBeNull();
});

const mailExists = async (sender, subject, date) => {
  const senderElem = await page.waitForSelector(
    `::-p-text(${sender})`);
  const subjectElem = await page.waitForSelector(
    `::-p-text(${subject})`);
  const dateElem = await page.waitForSelector(
    `::-p-text(${date})`);
  expect(senderElem !== null && subjectElem !== null && dateElem !== null);
}

test('First Inbox Mail', async () => {
  await mailExists(
    'Bari Guiden', 
    'Inverse 24/7 intranet', 
    'Jun 27');
});

test('Seventh Sent Mail', async () => {
  await page.click('::-p-aria(Select Sent)');
  await mailExists(
    'Phillip Warstall', 
    'Open-architected didactic knowledge user', 
    'Apr 11');
});

test('Third Trash Mail', async () => {
  await page.click('::-p-aria(Select Trash)');
  await mailExists(
    'Kynthia Perview to CSE186 Student', 
    'Up-sized solution-oriented task-force', 
    'Mar 25');
});

test('Delete Third Inbox Mail', async () => {
  await mailExists(
    'Berta Bubb', 
    'Implemented full-range functionalities', 
    'May 03');
  await page.click('::-p-aria(Delete mail from Berta Bubb received May 03)');
  await page.click('::-p-aria(Select Trash)');
  await mailExists(
    'Berta Bubb to CSE186 Student', 
    'Implemented full-range functionalities', 
    'May 03');
});

test('Delete Third Sent Mail', async () => {
  await page.click('::-p-aria(Select Sent)');
  await mailExists(
    'Kipper Josse', 
    'Decentralized empowering benchmark', 
    'Jul 03');
  await page.click('::-p-aria(Delete mail to Kipper Josse sent Jul 03)');
  await page.click('::-p-aria(Select Trash)');
  await mailExists(
    'CSE186 Student to Kipper Josse', 
    'Decentralized empowering benchmark', 
    'Jul 03');
});
