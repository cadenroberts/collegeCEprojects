const {Pool} = require('pg');
const {Router} = require('express');
const expressRouter = Router;
const router = expressRouter();

require('dotenv').config();

const pool = new Pool({
  host: 'localhost',
  port: 5432,
  database: process.env.POSTGRES_DB,
  user: process.env.POSTGRES_USER,
  password: process.env.POSTGRES_PASSWORD,
});

router.get('/mailbox', async (req, res) => {
  const result = await pool.query(
    'SELECT data->>\'name\' as name FROM mailbox ORDER BY name ASC');
  const mailboxes = result.rows.map((row) => row.name);
  return res.status(200).json(mailboxes);
});

router.get('/mail', async (req, res) => {
  const {mailbox} = req.query;
  const mailboxResult = await pool.query(
    'SELECT id FROM mailbox WHERE data->>\'name\' = $1',
    [mailbox],
  );
  if (mailboxResult.rows.length === 0) {
    return res.status(404).send();
  }
  const mailboxId = mailboxResult.rows[0].id;

  const mailResult = await pool.query(
    'SELECT id, data - \'content\' as data FROM mail WHERE mailbox = $1',
    [mailboxId],
  );
  const emails = mailResult.rows.map((row) => {
    const data = row.data;
    return {
      id: row.id,
      to: data.to,
      from: data.from,
      received: data.received,
      sent: data.sent,
      subject: data.subject,
    };
  });
  return res.status(200).json(emails);
});

router.put('/mail/:id', async (req, res) => {
  const {id} = req.params;
  const {mailbox} = req.query;
  if (mailbox === 'sent') {
    return res.status(403).send();
  }

  const emailResult = await pool.query(
    'SELECT id FROM mail WHERE id = $1', [id]);
  if (emailResult.rows.length === 0) {
    return res.status(404).send();
  }

  const mailboxResult = await pool.query(
    'SELECT id FROM mailbox WHERE data->>\'name\' = $1', [mailbox]);
  if (mailboxResult.rows.length === 0) {
    return res.status(404).send();
  }

  const mailboxId = mailboxResult.rows[0].id;
  await pool.query(
    'UPDATE mail SET mailbox = $1 WHERE id = $2', [mailboxId, id]);
  return res.status(204).send();
});

module.exports = router;
