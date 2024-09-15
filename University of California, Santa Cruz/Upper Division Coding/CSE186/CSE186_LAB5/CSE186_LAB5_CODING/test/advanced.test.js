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

// chatgpt.com inspired some of the testing methods, however my own edits have
// heavily overwritten so much that no particular test is not my own work

const supertest = require('supertest');
const http = require('http');
const fs = require('fs');
const path = require('path');
const app = require('../src/app');
let server;

beforeAll(() => {
  server = http.createServer(app);
  server.listen();
  request = supertest(server);
});

afterAll((done) => {
  server.close(done);
});

test('GET Invalid URL', async () => {
  await request.get('/v0/entirely-invalid-path-da-doo-de-dum-dum/')
    .expect(404);
});

/* Add additional tests below here */

test('GET /v0/mail - 200 get all emails', async () => {
  await request.get('/v0/mail').expect(200);
});

test('GET /v0/mail?mailbox={mailbox} - 200 emails from mailbox', async () => {
  await request.get('/v0/mail?mailbox=inbox').expect(200);
  await request.get('/v0/mail?mailbox=trash').expect(200);
  await request.get('/v0/mail?mailbox=sent').expect(200);
});

test('GET /v0/mail?mailbox=nonexistent - 404 mailbox not found', async () => {
  await request.get('/v0/mail?mailbox=nonexistent').expect(404);
});

test('GET /v0/mail/:id - 200 get email by ID', async () => {
  await request.get(`/v0/mail/${(await request.post('/v0/mail').send({
    'to-name': 'CSE186 Student',
    'to-email': 'cse186-student@ucsc.edu',
    'subject': 'Broderskab (Brotherhood)',
    'content': 'Duis aliquam convallis nunc.',
  }).expect(201)).body.id}`).expect(200);
});

test('GET /v0/mail/:id - 404 email not found', async () => {
  await request.get('/v0/mail/nonexistent-id').expect(404);
});

test('POST /v0/mail - 201', async () => {
  await request.post('/v0/mail').send({
    'to-name': 'CSE186 Student',
    'to-email': 'cse186-student@ucsc.edu',
    'subject': 'Broderskab (Brotherhood)',
    'content': 'Duis aliquam convallis nunc.',
  }).expect(201);
});

test('POST /v0/mail - 400 handle unexpected properties', async () => {
  expect((await request.post('/v0/mail').send({
    'to-name': 'CSE186 Student',
    'to-email': 'cse186-student@ucsc.edu',
    'subject': 'Broderskab (Brotherhood)',
    'content': 'Duis aliquam convallis nunc.',
    'unexpected': 'Unexpected property',
  }).expect(400)).text).toBe('Unexpected properties in request body');
});

test('PUT /v0/mail/:id - should return 404 if email not found', async () => {
  await request.put('/v0/mail/nonexistent-id?mailbox=inbox').expect(404);
});

test('PUT /v0/mail/:id - 409 sent mailbox email not found', async () => {
  const emailId = (await request.post('/v0/mail').send({
    'to-name': 'CSE186 Student',
    'to-email': 'cse186-student@ucsc.edu',
    'subject': 'Broderskab (Brotherhood)',
    'content': 'Duis aliquam convallis nunc.',
  }).expect(201)).body.id;
  await request.put(`/v0/mail/${emailId}?mailbox=inbox`).expect(204);
  await request.put(`/v0/mail/${emailId}?mailbox=sent`).expect(409);
});

test('PUT /v0/mail/:id - 204 new mailbox', async () => {
  let mailboxName = 'newmailbox';
  let counter = 2;
  while (true) {
    if ((await request.get('/v0/mail').query(
      {mailbox: mailboxName})).status === 404) break;
    mailboxName = `newmailbox${counter}`;
    counter++;
  }
  await request.put(`/v0/mail/${(await request.post('/v0/mail').send({
    'to-name': 'CSE186 Student',
    'to-email': 'cse186-student@ucsc.edu',
    'subject': 'Broderskab (Brotherhood)',
    'content': 'Duis aliquam convallis nunc.',
  }).expect(201)).body.id}`).query({mailbox: mailboxName}).expect(204);
});
