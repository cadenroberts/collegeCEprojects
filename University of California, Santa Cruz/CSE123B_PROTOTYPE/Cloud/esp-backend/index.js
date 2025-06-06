import express from 'express';
import { Pool } from 'pg';

const app = express();
app.use(express.json());

// Setup DB connection
const pool = new Pool({
    user: 'postgres',
    host: 'localhost',  
    database: 'postgres',
    password: 'postgres',
    port: 5432,
});

pool.query('SELECT version()', (err, result) => {
    if (err) {
        console.error('Failed to get version:', err.message);
    } else {
        console.log('Postgres version:', result.rows[0]);
    }
});



// 🔑 API Keys (UUID: API_KEY)
const API_KEYS = {
    '40eadfea-280e-463a-9e58-cc6137d1d7d4': 'THERAaliyaa_cred_2025',
};




// Upload route
app.post('/upload/:patient_id/:timestamp', async (req, res) => {
    const { patient_id, timestamp } = req.params;
    const apiKey = req.header('X-API-KEY');
    const data = req.body;

    console.log(`Incoming upload for patient ${patient_id}`);

    const expectedApiKey = API_KEYS[patient_id];  // ✅ FIRST

    console.log(`API key provided: ${apiKey}`);
    console.log(`Expected API key: ${expectedApiKey}`);

    // Check API Key
    if (!expectedApiKey || expectedApiKey !== apiKey) {
        return res.status(401).json({ error: 'Unauthorized' });
    }

    // Validate patient exists
    try {
        const check = await pool.query('SELECT id FROM patients WHERE id = $1', [patient_id]);
        if (check.rowCount === 0) {
            console.log(`Patient not found: ${patient_id}`);
            return res.status(404).json({ error: 'Patient not found' });
        }

        // Insert data
        await pool.query(
            'INSERT INTO sessions (patient_id, data) VALUES ($1, $2)',
            [patient_id, data]
        );

        console.log(`Data inserted for patient ${patient_id}`);
        res.json({ message: 'Upload complete', patient_id });
    } catch (err) {
        console.error('Error:', err.message);
        res.status(500).json({ error: err.message });
    }
});



app.listen(5000, () => {
    console.log('🚀 ESP Backend running on port 5000');
});
