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
const express = require('express');
const yaml = require('js-yaml');
const swaggerUi = require('swagger-ui-express');
const fs = require('fs');
const path = require('path');
const OpenApiValidator = require('express-openapi-validator');
const {v4: uuidv4} = require('uuid');
const app = express();
app.use(express.json());
app.use(express.urlencoded({extended: false}));

const apiSpec = path.join(__dirname, '../api/openapi.yaml');

const apidoc = yaml.load(fs.readFileSync(apiSpec, 'utf8'));
app.use('/v0/api-docs', swaggerUi.serve, swaggerUi.setup(apidoc));

app.use(
  OpenApiValidator.middleware({
    apiSpec: apiSpec,
    validateRequests: true,
    validateResponses: true,
  }),
);

// Your routes go here
// In-memory data store for emails
//

const mailboxes = {};

fs.readdirSync(path.join(__dirname, '../data')).forEach((file) => {
  if (file.endsWith('.json')) {
    mailboxes[path.basename(file, '.json')] = JSON.parse(
      fs.readFileSync(path.join(path.join(
        __dirname, '../data'), file), 'utf8'));
  }
});

const saveMailboxes = () => {
  Object.keys(mailboxes).forEach((box) => {
    fs.writeFileSync(path.join(path.join(
      __dirname, '../data'), `${box}.json`),
    JSON.stringify(mailboxes[box], null, 2));
  });
};

app.get('/v0/mail', (req, res) => {
  const m = req.query.mailbox;
  if (m) {
    if (!mailboxes[m]) {
      return res.status(404).send('Mailbox not found');
    }
    const response = {
      name: m,
      mail: mailboxes[m].map(({content, ...rest}) => rest),
    };
    return res.status(200).json([response]);
  } else {
    const response = Object.keys(mailboxes).map((name) => ({
      name,
      mail: mailboxes[name].map(({content, ...rest}) => rest),
    }));
    return res.status(200).json(response);
  }
});

app.get('/v0/mail/:id', (req, res) => {
  const email = Object.values(mailboxes).flat().find((mail) => mail.id ===
    req.params.id);
  if (!email) {
    return res.status(404).send('Email not found');
  }
  res.status(200).json(email);
});

app.post('/v0/mail', (req, res) => {
  const {'to-name': toName, 'to-email': toEmail,
    subject, content, ...rest} = req.body;
  if (Object.keys(rest).length > 0) {
    return res.status(400).send('Unexpected properties in request body');
  }
  const newEmail = {
    id: uuidv4(),
    toName,
    toEmail,
    fromName: 'Cherye O\'Loughane',
    fromEmail: 'coloughane0@nymag.com',
    subject,
    received: new Date().toISOString(),
    content,
  };
  mailboxes.sent.push(newEmail);
  saveMailboxes();
  res.status(201).json(newEmail);
});

app.put('/v0/mail/:id', (req, res) => {
  const {mailbox} = req.query;
  const email = Object.values(mailboxes).flat().find(
    (mail) => mail.id === req.params.id);
  if (!mailboxes[mailbox]) {
    mailboxes[mailbox] = [];
  }
  if (!email) {
    return res.status(404).send('Email not found');
  }
  if (mailbox === 'sent' && !mailboxes.sent.includes(email)) {
    return res.status(409).send('Cannot move to sent mailbox');
  }
  Object.keys(mailboxes).forEach((box) => {
    mailboxes[box] = mailboxes[box].filter(
      (mail) => mail.id !== req.params.id);
  });
  mailboxes[mailbox].push(email);
  saveMailboxes();
  res.status(204).send();
});

app.use((err, req, res, next) => {
  res.status(err.status).json({
    message: err.message,
    errors: err.errors,
    status: err.status,
  });
});

module.exports = app;
