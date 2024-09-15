const {Pool} = require('pg');
const {Router} = require('express');
const expressRouter = Router;
const router = expressRouter();

// Database connection pool
const pool = new Pool({
  host: 'localhost',
  port: 5432,
  database: process.env.POSTGRES_DB,
  user: process.env.POSTGRES_USER,
  password: process.env.POSTGRES_PASSWORD,
});

router.get('/mail', async (req, res) => {
  const {mailbox, from} = req.query;
  let emails;

  if (mailbox) {
    const result = await pool.query(
      `SELECT m.id, m.mail->>'to' AS to, m.mail->>'from' AS from,
       m.mail->>'subject' AS subject, 
              m.mail->>'sent' AS sent, m.mail->>'received' AS received 
       FROM mail m 
       JOIN mailbox b ON m.mailbox = b.id 
       WHERE b.name = $1`,
      [mailbox],
    );
    if (result.rows.length === 0) {
      return res.status(404).send('Mailbox not found');
    }
    emails = result.rows.map((row) => ({...row, to: JSON.parse(row.to),
      from: JSON.parse(row.from)}));
    res.status(200).json([{name: mailbox, mail: emails}]);
  } else if (from) {
    const isEmail = /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(from);
    let query;
    if (isEmail) {
      query = `SELECT b.name AS mailbox,
                 jsonb_agg(
                   jsonb_build_object(
                     'id', m.id,
                     'to', m.mail->'to',
                     'from', m.mail->'from',
                     'subject', m.mail->'subject',
                     'sent', m.mail->'sent',
                     'received', m.mail->'received'
                   ) - 'content'
                 ) AS emails
                 FROM mail m 
                 JOIN mailbox b ON m.mailbox = b.id 
                 WHERE LOWER(m.mail->'from'->>'email') = LOWER($1)
                 GROUP BY b.name`;
    } else {
      query = `SELECT b.name AS mailbox,
                 jsonb_agg(
                   jsonb_build_object(
                     'id', m.id,
                     'to', m.mail->'to',
                     'from', m.mail->'from',
                     'subject', m.mail->'subject',
                     'sent', m.mail->'sent',
                     'received', m.mail->'received'
                   ) - 'content'
                 ) AS emails
                 FROM mail m 
                 JOIN mailbox b ON m.mailbox = b.id 
                 WHERE LOWER(m.mail->'from'->>'name') LIKE LOWER($1)
                 GROUP BY b.name`;
    }


    const result = await pool.query(query, [isEmail ? from : `%${from}%`]);
    if (result.rows.length === 0) {
      return res.status(404).send('No emails found from the specified sender');
    }
    return res.status(200).json(result.rows.map((row) => ({name: row.mailbox,
      mail: row.emails})));
  } else {
    const result = await pool.query(
      `SELECT b.name AS name, jsonb_agg(
            jsonb_build_object(
              'id', m.id,
              'to', m.mail->'to',
              'from', m.mail->'from',
              'subject', m.mail->'subject',
              'sent', m.mail->'sent',
              'received', m.mail->'received'
            ) - 'content'
          ) AS mail 
         FROM mail m 
         JOIN mailbox b ON m.mailbox = b.id 
         GROUP BY b.name`,
    );
    res.status(200).json(result.rows);
  }
});

router.get('/mail/:id', async (req, res) => {
  const {id} = req.params;

  const result = await pool.query(
    'SELECT id, mail FROM mail WHERE id = $1',
    [id],
  );

  if (result.rows.length === 0) {
    return res.status(404).send('Email not found');
  }

  const email = result.rows[0].mail;
  const responseEmail = {id: result.rows[0].id, ...email};
  res.status(200).json(responseEmail);
});

router.post('/mail', async (req, res) => {
  const {to, subject, content} = req.body;
  const {name: toName, email: toEmail} = to;

  if (!toName || !toEmail || !subject || !content) {
    return res.status(400).send('Missing required fields');
  }

  const allowedFields = ['to', 'subject', 'content'];
  const extraFields = Object.keys(req.body).filter(
    (key) => !allowedFields.includes(key));
  if (extraFields.length > 0) {
    return res.status(400).send('Unexpected properties in request body');
  }

  const result = await pool.query(
    'SELECT id FROM mailbox WHERE name = $1',
    ['sent'],
  );
  const mailboxId = result.rows[0].id;
  const dateSent = new Date().toISOString();
  const newEmail = {
    to: {name: toName, email: toEmail},
    from: {name: 'CSE186 Student', email: 'CSE186student@ucsc.edu'},
    subject,
    sent: dateSent,
    received: dateSent,
    content,
  };

  const insertResult = await pool.query(
    `INSERT INTO mail (mailbox, mail) 
     VALUES ($1, $2) 
     RETURNING id, mail`,
    [mailboxId, newEmail],
  );

  const responseEmail = {id: insertResult.rows[0].id,
    ...insertResult.rows[0].mail};
  res.status(201).json(responseEmail);
});

router.put('/mail/:id', async (req, res) => {
  const {mailbox} = req.query;
  const emailId = req.params.id;

  const emailResult = await pool.query(
    'SELECT id FROM mail WHERE id = $1',
    [emailId],
  );

  if (emailResult.rows.length === 0) {
    return res.status(404).send('Email not found');
  }

  const currentMailboxId = emailResult.rows[0].mailbox;

  if (mailbox === 'sent') {
    const currentMailboxNameResult = await pool.query(
      'SELECT name FROM mailbox WHERE id = $1',
      [currentMailboxId],
    );

    if (currentMailboxNameResult.rows.length === 0 || currentMailboxNameResult.
      rows[0].name !== 'sent') {
      return res.status(409).send('Cannot move to sent mailbox');
    }
  }

  const mailboxResult = await pool.query(
    'SELECT id FROM mailbox WHERE name = $1',
    [mailbox],
  );

  let mailboxId;
  if (mailboxResult.rows.length === 0) {
    const newMailboxResult = await pool.query(
      'INSERT INTO mailbox (name) VALUES ($1) RETURNING id',
      [mailbox],
    );
    mailboxId = newMailboxResult.rows[0].id;
  } else {
    mailboxId = mailboxResult.rows[0].id;
  }

  await pool.query(
    'UPDATE mail SET mailbox = $1 WHERE id = $2',
    [mailboxId, emailId],
  );

  res.status(204).send();
});

module.exports = router;
