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

const delay = (time) => new Promise((resolve) => setTimeout(resolve, time));

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
  setImmediate(function() {
    frontend.emit('close');
  });
});

beforeEach(async () => {
  browser = await puppeteer.launch({
    headless: true,
    // Use these settings if you want to see the browser
    // headless: false,
    // slowMo: 40,
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

test('Login Functionality', async () => {
  await page.click('::-p-aria(Login)');
  expect(page.url()).toBe('http://localhost:3000/');
  await page.type('input[name="email"]', 'a@abc.com');
  await page.click('::-p-aria(Login)');
  expect(page.url()).toBe('http://localhost:3000/');
  await page.type('input[name="password"]', 'a');
  await page.click('[aria-label="rememberMe"]');
  await page.click('::-p-aria(Login)');
  await delay(1000);
  expect(page.url()).toBe('http://localhost:3000/home');
  await page.goto('http://localhost:3000/');
  expect(
    await page.$eval('input[name="email"]', (el) => el.value),
  ).toBe('a@abc.com');
  expect(
    await page.$eval('input[name="password"]', (el) => el.value),
  ).toBe('');
  await page.click('[aria-label="rememberMe"]');
  await page.type('input[name="password"]', 'a');
  await page.click('::-p-aria(Login)');
  await delay(1000);
  expect(page.url()).toBe('http://localhost:3000/home');
  await page.goto('http://localhost:3000/');
  expect(
    await page.$eval('input[name="email"]', (el) => el.value),
  ).toBe('');
  expect(
    await page.$eval('input[name="password"]', (el) => el.value),
  ).toBe('');
  await page.type('input[name="email"]', 'a@abc.com');
  await page.type('input[name="password"]', 'b');
  await page.click('::-p-aria(Login)');
  expect(page.url()).toBe('http://localhost:3000/');
});


test('Workspace Functionality', async () => {
  await page.type('input[name="email"]', 'a@abc.com');
  await page.type('input[name="password"]', 'a');
  await page.click('::-p-aria(Login)');
  await delay(1000);
  await page.click('::-p-aria(workspaceList)');
  await delay(1000);
  await page.click('::-p-aria(Molly Workspace)');
  await delay(1000);
  const channelText = await page.evaluate(() => document.body.innerText);
  expect(channelText).toContain('Molly Workspace');
  expect(channelText).toContain('Molly Books');
  expect(channelText).toContain('Molly Magazines');
  await page.click('::-p-aria(workspaceList)');
  const workspaceText = await page.evaluate(() => document.body.innerText);
  expect(workspaceText).toContain('User A Workspace');
  expect(workspaceText).toContain('Molly Workspace');
  expect(workspaceText).toContain('Anna Workspace');
});

test('Message Functionality (and back)', async () => {
  await page.type('input[name="email"]', 'a@abc.com');
  await page.type('input[name="password"]', 'a');
  await page.click('::-p-aria(Login)');
  await delay(1000);
  await page.click('::-p-aria(workspaceList)');
  await delay(1000);
  await page.click('::-p-aria(Molly Workspace)');
  await delay(1000);
  await page.click('::-p-aria(Channel Molly Books)');
  await delay(1000);
  const messageText = await page.evaluate(() => document.body.innerText);
  expect(messageText).toContain('Molly Books');
  expect(messageText).toContain('Jun 13');
  expect(messageText).toContain('07:15');
  expect(messageText).toContain('User A');
  expect(messageText).toContain('I miss reading.');
  expect(messageText).toContain('07:16');
  expect(messageText).toContain('Anna Admin');
  expect(messageText).toContain(
    'My favorite book is Where the Red Fern Grows.');
  expect(messageText).toContain('Jun 14');
  expect(messageText).toContain('07:16');
  expect(messageText).toContain('Molly Member');
  expect(messageText).toContain('Hey, does anyone love Harry Potter too?');
  await page.click('::-p-aria(back)');
  const channelText = await page.evaluate(() => document.body.innerText);
  expect(channelText).toContain('Molly Books');
  expect(channelText).toContain('Molly Magazines');
  expect(channelText).toContain('Molly Workspace');
});

test('Thread Functionality (and back button)', async () => {
  await page.type('input[name="email"]', 'a@abc.com');
  await page.type('input[name="password"]', 'a');
  await page.click('::-p-aria(Login)');
  await delay(1000);
  await page.click('::-p-aria(workspaceList)');
  await delay(1000);
  await page.click('::-p-aria(Molly Workspace)');
  await delay(1000);
  await page.click('::-p-aria(Channel Molly Books)');
  await delay(1000);
  await page.click('::-p-aria(Hey, does anyone love Harry Potter too?)');
  await delay(1000);
  const threadText = await page.evaluate(() => document.body.innerText);
  expect(threadText).toContain('Jun 14');
  expect(threadText).toContain('07:16');
  expect(threadText).toContain('Molly Member');
  expect(threadText).toContain('Hey, does anyone love Harry Potter too?');
  expect(threadText).toContain('07:25');
  expect(threadText).toContain('Anna Admin');
  expect(threadText).toContain('The Chamber of Secrets is my favorite!');
  expect(threadText).toContain('07:35');
  expect(threadText).toContain('User A');
  expect(threadText).toContain(
    'Yes, though I do like the movies more than the books, sue me!');
  await page.click('::-p-aria(back)');
  await delay(1000);
  const messageText = await page.evaluate(() => document.body.innerText);
  expect(messageText).toContain('Molly Books');
  expect(messageText).toContain('Jun 13');
  expect(messageText).toContain('07:15');
  expect(messageText).toContain('User A');
  expect(messageText).toContain('I miss reading.');
  expect(messageText).toContain('07:16');
  expect(messageText).toContain('Anna Admin');
  expect(messageText).toContain(
    'My favorite book is Where the Red Fern Grows.');
  expect(messageText).toContain('Jun 14');
  expect(messageText).toContain('07:16');
  expect(messageText).toContain('Molly Member');
  expect(messageText).toContain('Hey, does anyone love Harry Potter too?');
});

test('Home, Logout, and Unauthorized Functionality', async () => {
  await page.goto('http://localhost:3000/home');
  await delay(1000);
  expect(page.url()).toBe('http://localhost:3000/unauthorized');
  const unauthorizedText = await page.evaluate(() => document.body.innerText);
  expect(unauthorizedText).toContain('Unauthorized Access');
  expect(unauthorizedText).toContain('You do not have access to this page.');
  expect(unauthorizedText).toContain('BACK TO LOGIN');
  await page.click('[aria-label="backToLogin"]');
  await delay(1000);
  expect(page.url()).toBe('http://localhost:3000/');
  await page.type('input[name="email"]', 'a@abc.com');
  await page.type('input[name="password"]', 'a');
  await page.click('::-p-aria(Login)');
  await delay(1000);
  await page.click('::-p-aria(workspaceList)');
  await delay(1000);
  await page.click('::-p-aria(Molly Workspace)');
  await delay(1000);
  await page.click('::-p-aria(Channel Molly Books)');
  await delay(1000);
  await page.click('::-p-aria(home)');
  await delay(1000);
  await page.click('::-p-aria(workspaceList)');
  await delay(1000);
  await page.click('::-p-aria(Molly Workspace)');
  await delay(1000);
  const homeText = await page.evaluate(() => document.body.innerText);
  expect(homeText).toContain('Molly Workspace');
  await page.click('::-p-aria(Channel Molly Books)');
  await delay(1000);
  await page.click('::-p-aria(Hey, does anyone love Harry Potter too?)');
  await delay(1000);
  await page.click('::-p-aria(home)');
  await delay(1000);
  const home2Text = await page.evaluate(() => document.body.innerText);
  expect(home2Text).toContain('Molly Workspace');
  await page.click('::-p-aria(logout)');
  await delay(1000);
  await page.goto('http://localhost:3000/home');
  await delay(1000);
  expect(page.url()).toBe('http://localhost:3000/unauthorized');
});
