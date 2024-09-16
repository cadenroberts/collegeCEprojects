const supertest = require('supertest');
const http = require('http');
const {Pool} = require('pg');
const db = require('../test/db');
const app = require('../src/app');

let server;

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

const pool = new Pool({
  host: 'localhost',
  port: 5432,
  database: process.env.POSTGRES_DB,
  user: process.env.POSTGRES_USER,
  password: process.env.POSTGRES_PASSWORD,
});

test('POST /v0/mail and verify persistence', async () => {
  const emailResponse = await request
    .post('/v0/mail')
    .send({
      to: {
        name: 'CSE186 Student',
        email: 'cse186-student@ucsc.edu',
      },
      subject: 'Test Email',
      content: 'This is a test email.',
    })
    .expect(201);

  const emailId = emailResponse.body.id;

  let result = await pool.query('SELECT * FROM mail WHERE id = $1', [emailId]);
  expect(result.rows.length).toBe(1);

  await new Promise((resolve) => {
    server.close(() => {
      server = http.createServer(app);
      server.listen(resolve);
      request = supertest(server);
    });
  });

  result = await pool.query('SELECT * FROM mail WHERE id = $1', [emailId]);
  expect(result.rows.length).toBe(1);
});
