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

test('POST /v0/mail and verify persistence', async () => {
  const emailId = (await request.post('/v0/mail').send({
    'to-name': 'CSE186 Student',
    'to-email': 'cse186-student@ucsc.edu',
    'subject': 'Broderskab (Brotherhood)',
    'content': 'Duis aliquam convallis nunc.',
  }).expect(201)).body.id;
  expect(JSON.parse(fs.readFileSync(path.join(path.join(__dirname, '../data'),
    `sent.json`), 'utf8')).some((mail) => mail.id === emailId)).toBe(true);
  await new Promise((resolve) => {
    server.close(() => {
      server = http.createServer(app);
      server.listen(resolve);
      request = supertest(server);
    });
  });
  expect(JSON.parse(fs.readFileSync(path.join(path.join(__dirname, '../data'),
    `sent.json`), 'utf8')).some((mail) => mail.id === emailId)).toBe(true);
});

