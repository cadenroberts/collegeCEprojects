/*
 * Copyright (C) 2022-2024 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */
/*
 * *****************************************************
 * DO NOT MODIFY THIS FILE
 * *****************************************************
 */
const fs = require('fs');
const {Pool} = require('pg');

require('dotenv').config();
process.env.POSTGRES_DB='test';

const pool = new Pool({
  host: 'localhost',
  port: 5432,
  database: process.env.POSTGRES_DB,
  user: process.env.POSTGRES_USER,
  password: process.env.POSTGRES_PASSWORD,
});

const run = async (file) => {
  const content = fs.readFileSync(file, 'utf8');
  const statements = content.split(/\r?\n/);
  for (statement of statements) {
    await pool.query(statement);
  }
};

exports.reset = async () => {
  await run('sql/schema.sql');
  await run('sql/data.sql');
};

exports.shutdown = () => {
  pool.end(() => {
    // console.log('pool has ended');
  });
};
