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

test('POST Valid Username', async () =>{
  const validUser = {
    email: 'a@abc.com',
    password: 'a',
  };
  const response = await request.post(`/v0/`)
    .set('Content-Type', 'application/json')
    .send(validUser);
  expect(response.status).toBe(200);
});

test('POST Invalid Password', async () =>{
  const invalidUser = {
    email: 'a@abc.com',
    password: 'b',
  };
  const response = await request.post(`/v0/`)
    .set('Content-Type', 'application/json')
    .send(invalidUser);
  expect(response.status).toBe(401);
});

test('POST Invalid Email', async () =>{
  const invalidUser = {
    email: 'a@ac.com',
    password: 'b',
  };
  const response = await request.post(`/v0/`)
    .set('Content-Type', 'application/json')
    .send(invalidUser);
  expect(response.status).toBe(401);
});

test('POST Nonsense', async () =>{
  const invalidUser = 'nonsense';
  const response = await request.post(`/v0/`)
    .set('Content-Type', 'application/json')
    .send(invalidUser);
  expect(response.status).toBe(400);
});

test('GET /home/workspaces with valid token return workspaces', async () => {
  const validUser = {
    email: 'a@abc.com', // Replace with a valid email from your test database
    password: 'a', // Replace with a valid password from your test database
  };
  const validToken = (await request.post(`/v0/`)
    .set('Content-Type', 'application/json')
    .send(validUser)).body.accessToken;
  const response = await request
    .get('/v0/home/workspaces')
    .set('Authorization', `Bearer ${validToken}`)
    .expect(200);
  const workspaces = JSON.parse(response.text);
  expect(workspaces).toBeInstanceOf(Array);
  workspaces.forEach((workspace) => {
    expect(workspace).toHaveProperty('id');
    expect(workspace).toHaveProperty('owner_id');
    expect(workspace).toHaveProperty('data');
    expect(workspace.data).toHaveProperty('name');
  });
});

test('GET /home/workspaces with invalid token', async () => {
  const token = 'invalid';
  await request.get('/v0/home/workspaces')
    .set('Authorization', `Bearer ${token}`)
    .expect(403);
});

test('GET /home/ /channels + /messages + /threads',
  async () => {
    const validUser = {
      email: 'a@abc.com',
      password: 'a',
    };
    const validToken = (await request.post(`/v0/`)
      .set('Content-Type', 'application/json')
      .send(validUser)).body.accessToken;
    const response = await request
      .get('/v0/home/workspaces/')
      .set('Authorization', `Bearer ${validToken}`)
      .expect(200);
    const workspaces = JSON.parse(response.text);
    const response2 = await request
      .get(`/v0/home/${workspaces[0].id}/channels`)
      .set('Authorization', `Bearer ${validToken}`)
      .expect(200);
    const channels = response2.body;
    expect(channels).toBeInstanceOf(Array);
    channels.forEach((channel) => {
      expect(channel).toHaveProperty('id');
      expect(channel).toHaveProperty('workspace_id');
      expect(channel).toHaveProperty('data');
      expect(channel.data).toHaveProperty('name');
    });
    const response3 = await request
      .get(`/v0/home/${channels[0].id}/messages`)
      .set('Authorization', `Bearer ${validToken}`)
      .expect(200);
    const messages = response3.body;
    expect(messages).toBeInstanceOf(Array);
    messages.forEach((message) => {
      expect(message).toHaveProperty('id');
      expect(message).toHaveProperty('channel_id');
      expect(message).toHaveProperty('data');
      expect(message.data).toHaveProperty('from');
      expect(message.data).toHaveProperty('date');
      expect(message.data).toHaveProperty('content');
    });
    const response4 = await request
      .get(`/v0/home/${messages[0].id}/threads`)
      .set('Authorization', `Bearer ${validToken}`)
      .expect(200);
    const threads = response4.body;
    expect(threads).toBeInstanceOf(Array);
    threads.forEach((thread) => {
      expect(thread).toHaveProperty('id');
      expect(thread).toHaveProperty('message_id');
      expect(thread).toHaveProperty('data');
      expect(thread.data).toHaveProperty('from');
      expect(thread.data).toHaveProperty('date');
      expect(thread.data).toHaveProperty('content');
    });
  });
