const supertest = require('supertest');
const http = require('http');
const db = require('../test/db');
const app = require('../src/app');
const {Pool} = require('pg');

let server;

const pool = new Pool({
  host: 'localhost',
  port: 5432,
  database: process.env.POSTGRES_DB,
  user: process.env.POSTGRES_USER,
  password: process.env.POSTGRES_PASSWORD,
});

beforeAll(() => {
  server = http.createServer(app);
  server.listen();
  request = supertest(server);
  return db.reset();
});

afterAll((done) => {
  server.close(done);
  db.close();
});

test('GET Invalid URL', async () => {
  await request.get('/v0/entirely-invalid-path-da-doo-de-dum-dum/')
    .expect(404);
});

test('GET /v0/mail - should return all emails', async () => {
  const response = await request.get('/v0/mail').expect(200);
  expect(response.body).toBeInstanceOf(Array);
});

test('GET /v0/mail - should return all emails', async () => {
  const response = await request.get('/v0/mail?from=cse186').expect(200);
  expect(response.body).toBeInstanceOf(Array);
});

test('GET /v0/mail - from name', async () => {
  const newEmail = {
    to: {name: 'Recipient Name', email: 'recipient@example.com'},
    subject: 'Test Subject',
    content: 'Test Content',
  };
  await request.post('/v0/mail').send(newEmail).expect(201);
  await request.get('/v0/mail?from=CSE').expect(200);
});

test('GET /v0/mail - no emails from sender', async () => {
  await request.get('/v0/mail?from=randorando').expect(404);
});

test('GET /v0/mail - from email', async () => {
  const newEmail = {
    to: {name: 'Recipient Name', email: 'recipient@example.com'},
    subject: 'Test Subject',
    content: 'Test Content',
  };
  await request.post('/v0/mail').send(newEmail).expect(201);
  await request.get('/v0/mail?from=CSE186student%40ucsc.edu').expect(200);
});

test('GET /v0/mail?mailbox=inbox - should return emails in inbox', async () => {
  const response = await request.get('/v0/mail?mailbox=inbox').expect(200);
  expect(response.body[0].name).toBe('inbox');
});

test('GET /v0/mail?mailbox=nonsense - should return 404', async () => {
  await request.get('/v0/mail?mailbox=nonsense').expect(404);
});

test('POST /v0/mail - should create a new email', async () => {
  const newEmail = {
    to: {
      name: 'CSE186 Student',
      email: 'cse186-student@ucsc.edu',
    },
    subject: 'Test Email',
    content: 'This is a test email.',
  };
  const response = await request.post('/v0/mail').send(newEmail).expect(201);
  expect(response.body.id).toBeDefined();
  expect(response.body.to.email).toBe(newEmail.to.email);
});

test('POST /v0/mail - email with blank field', async () => {
  const newEmail = {
    to: {
      name: 'CSE186 Student',
      email: 'cse186-student@ucsc.edu',
    },
    subject: '',
    content: 'This is a test email.',
  };
  await request.post('/v0/mail').send(newEmail).expect(400);
});

test('POST /v0/mail - email with extra field', async () => {
  const newEmail = {
    to: {
      name: 'CSE186 Student',
      email: 'cse186-student@ucsc.edu',
    },
    subject: 'Hi',
    content: 'This is a test email.',
    n: 'n',
  };
  await request.post('/v0/mail').send(newEmail).expect(400);
});

test('GET /v0/mail/:id - should return an email by Id', async () => {
  const emailResponse = await request.post('/v0/mail').send({
    to: {
      name: 'CSE186 Student',
      email: 'cse186-student@ucsc.edu',
    },
    subject: 'Test Email',
    content: 'This is a test email.',
  }).expect(201);

  const response = await request.get(`/v0/mail/${emailResponse.body.id}`).
    expect(200);
  expect(response.body.id).toBe(emailResponse.body.id);
});

test('PUT /v0/mail/:id?mailbox=trash - move email to trash', async () => {
  const emailResponse = await request.post('/v0/mail').send({
    to: {
      name: 'CSE186 Student',
      email: 'cse186-student@ucsc.edu',
    },
    subject: 'Test Email',
    content: 'This is a test email.',
  }).expect(201);

  await request.put(`/v0/mail/${emailResponse.body.id}?mailbox=trash`).
    expect(204);

  const response = await request.get('/v0/mail?mailbox=trash').expect(200);
  expect(response.body[0].mail.find((mail) => mail.id ===
    emailResponse.body.id)).toBeDefined();
});

test('PUT /v0/mail/:id?mailbox=trash - move nonexistent email', async () => {
  const mockUuid = '123e4567-e89b-12d3-a456-426614174000';
  await request.put(`/v0/mail/${mockUuid}?mailbox=trash`).expect(404);
});

test('PUT /v0/mail/:id?mailbox=trash - move nonexistent email', async () => {
  const mockUuid = '123e4567-e89b-12d3-a456-426614174000';
  await request.get(`/v0/mail/${mockUuid}`).expect(404);
});

test('PUT /v0/mail/:id - 409 sent mailbox email not found', async () => {
  const newEmail = {
    to: {name: 'CSE186 Student', email: 'CSE186student@ucsc.edu'},
    subject: 'Broderskab (Brotherhood)',
    content: 'Duis aliquam convallis nunc.',
  };
  const emailId = (await request.post('/v0/mail').
    send(newEmail).expect(201)).body.id;
  await request.put(`/v0/mail/${emailId}?mailbox=inbox`).expect(204);
  await request.put(`/v0/mail/${emailId}?mailbox=sent`).expect(409);
});

test('PUT /v0/mail/:id - create mailbox if it does not exist', async () => {
  const newEmail = {
    to: {name: 'CSE186 Student', email: 'CSE186student@ucsc.edu'},
    subject: 'Broderskab (Brotherhood)',
    content: 'Duis aliquam convallis nunc.',
  };
  const emailResponse = await request.post('/v0/mail').
    send(newEmail).expect(201);
  const emailId = emailResponse.body.id;
  const newMailbox = 'newMailbox';
  await request.put(`/v0/mail/${emailId}?mailbox=${newMailbox}`).expect(204);
  const response = await request.get(`/v0/mail/${emailId}`).expect(200);
  expect(response.body.id).toBe(emailId);
  const mailboxResult = await pool.query(
    'SELECT * FROM mailbox WHERE name = $1',
    [newMailbox]);
  expect(mailboxResult.rows.length).toBe(1);
  expect(mailboxResult.rows[0].name).toBe(newMailbox);

  const mailResult = await pool.query(
    'SELECT * FROM mail WHERE id = $1 AND mailbox = $2',
    [emailId, mailboxResult.rows[0].id]);
  expect(mailResult.rows.length).toBe(1);
});
