const supertest = require('supertest');
const http = require('http');
const app = require('../src/app');
const db = require('../test/db');
let server;

beforeAll(() => {
  server = http.createServer(app);
  server.listen();
  request = supertest(server);
  return db.reset();
});

afterAll((done) => {
  server.close(done);
  db.shutdown();
});

test('GET Invalid URL', async () => {
  await request.get('/v0/entirely-invalid-path-da-doo-de-dum-dum/')
    .expect(404);
});

test('GET Mailboxes', async () => {
  await request.get('/v0/mailbox').expect(200);
});


test('GET Inbox Trash Sent', async () => {
  await request.get('/v0/mail?mailbox=inbox').expect(200);
  await request.get('/v0/mail?mailbox=trash').expect(200);
  await request.get('/v0/mail?mailbox=sent').expect(200);
});

test('GET Bad Inbox', async () => {
  await request.get('/v0/mail?mailbox=badinbox').expect(404);
});

test('PUT Inbox to Trash', async () => {
  const inboxResponse = await request.get(
    '/v0/mail?mailbox=inbox').expect(200);
  const emailId = inboxResponse.body[0].id;
  await request.put(`/v0/mail/${emailId}?mailbox=trash`).expect(204);
  const trashResponse = await request.get(
    '/v0/mail?mailbox=trash').expect(200);
  const isEmailInTrash = trashResponse.body.some(
    (email) => email.id === emailId);
  expect(isEmailInTrash).toBe(true);
});

test('PUT Inbox to Bad Inbox', async () => {
  const inboxResponse = await request.get(
    '/v0/mail?mailbox=inbox').expect(200);
  const emailId = inboxResponse.body[0].id;
  await request.put(`/v0/mail/${emailId}?mailbox=badinbox`).expect(404);
  const inboxVerifyResponse = await request.get(
    '/v0/mail?mailbox=inbox').expect(200);
  const isEmailInInbox = inboxVerifyResponse.body.some(
    (email) => email.id === emailId);
  expect(isEmailInInbox).toBe(true);
});

test('PUT Inbox to Sent', async () => {
  const inboxResponse = await request.get(
    '/v0/mail?mailbox=inbox').expect(200);
  const emailId = inboxResponse.body[0].id;
  await request.put(`/v0/mail/${emailId}?mailbox=sent`).expect(403);
  const inboxVerifyResponse = await request.get(
    '/v0/mail?mailbox=inbox').expect(200);
  const isEmailInInbox = inboxVerifyResponse.body.some(
    (email) => email.id === emailId);
  expect(isEmailInInbox).toBe(true);
});

test('PUT Invalid Email to Inbox', async () => {
  const invalidEmailId = '123e4567-e89b-12d3-a456-426614174000';
  await request.put(`/v0/mail/${invalidEmailId}?mailbox=inbox`).expect(404);
});
