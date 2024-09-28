/*
 * Copyright (C) 2022-2024 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */
const express = require('express');
const cors = require('cors');
const yaml = require('js-yaml');
const swaggerUi = require('swagger-ui-express');
const fs = require('fs');
const path = require('path');
const OpenApiValidator = require('express-openapi-validator');
const {Pool} = require('pg');
const secrets = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlbWFpbCI6ImFubmFAYm'+
  '9va3MuY29tIiwicm9sZSI6ImFkbWluIiwiaWF0IjoxNjA2Mjc3MDAxLCJleHAiOjE2MDYyNzc'+
  'wNjF9.1nwY0lDMGrb7AUFFgSaYd4Q7Tzr-BjABclmoKZOqmr4';
const jwt = require('jsonwebtoken');
const bcrypt = require('bcrypt');
require('dotenv').config();

const pool = new Pool({
  host: 'localhost',
  port: 5432,
  database: process.env.POSTGRES_DB,
  user: process.env.POSTGRES_USER,
  password: process.env.POSTGRES_PASSWORD,
});

const login = async (req, res) => {
  const {email, password} = req.body;
  const result = await pool.query(
    'SELECT id, data FROM users WHERE data->>\'email\' = $1', [email]);
  if (result.rows[0]) {
    const user = result.rows[0];
    if (user.data.email == email &&
      bcrypt.compareSync(password, user.data.password)) {
      const accessToken = jwt.sign(
        {id: user.id},
        secrets, {
          expiresIn: '30m',
          algorithm: 'HS256',
        });
      res.status(200).json({id: user.id, accessToken: accessToken});
    } else {
      res.status(401).send('Invalid credentials');
    }
  } else {
    res.status(401).send('Invalid credentials');
  }
};

const check = (req, res, next) => {
  const token = req.headers.authorization.split(' ')[1];
  jwt.verify(token, secrets, (err, user) => {
    if (err) {
      return res.sendStatus(403);
    }
    req.user = user;
    next();
  });
};
// req.user.id SELECT
const getWorkspaces = async (req, res) => {
  // const id = req.user.id; id in results of select, or owner id
  const id = req.user.id;
  const result = await pool.query(`
    SELECT DISTINCT w.*
    FROM workspaces w
    LEFT JOIN user_workspaces uw ON w.id = uw.workspace_id
    WHERE w.owner_id = $1 OR uw.user_id = $1  
  `, [id]);
  res.status(200).send(JSON.stringify(result.rows));
};

const getChannels = async (req, res) => {
  const workspaceId = req.params.workspace_id;
  const result = await pool.query(
    'SELECT * FROM channels WHERE workspace_id = $1', [workspaceId]);
  res.status(200).json(result.rows);
};

const getMessages = async (req, res) => {
  const channelId = req.params.channel_id;
  const result = await pool.query(`
  SELECT * FROM messages
  WHERE channel_id = $1
  ORDER BY (data->>'date')::timestamp
  `, [channelId]);
  res.status(200).json(result.rows);
};

const getThreads = async (req, res) => {
  const messageId = req.params.message_id;
  const result = await pool.query(`
  SELECT * FROM threads
  WHERE message_id = $1
  ORDER BY (data->>'date')::timestamp
  `, [messageId]);
  res.status(200).json(result.rows);
};

const app = express();
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({extended: false}));

const apiSpec = path.join(__dirname, '../api/openapi.yaml');
const apidoc = yaml.load(fs.readFileSync(apiSpec, 'utf8'));

app.use(
  '/v0/api-docs',
  swaggerUi.serve,
  swaggerUi.setup(apidoc),
);

app.use(
  OpenApiValidator.middleware({
    apiSpec: apiSpec,
    validateRequests: true,
    validateResponses: true,
  }),
);

// Your Express routes go here
app.post('/v0/', login);
app.get('/v0/home/workspaces', check, getWorkspaces);
app.get('/v0/home/:workspace_id/channels', check, getChannels);
app.get('/v0/home/:channel_id/messages', check, getMessages);
app.get('/v0/home/:message_id/threads', check, getThreads);

//
app.use((err, req, res, next) => {
  res.status(err.status).json({
    message: err.message,
    errors: err.errors,
    status: err.status,
  });
});

module.exports = app;
