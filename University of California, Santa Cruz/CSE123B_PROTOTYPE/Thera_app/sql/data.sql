-- Clean up existing data
DELETE FROM messages;
DELETE FROM doctor_patients;
DELETE FROM patient_devices;
DELETE FROM doctors;
DELETE FROM patients;
DELETE FROM users;
DELETE FROM devices;
DELETE FROM results;
DELETE FROM assignments;
DELETE FROM runner;

-- Insert users (patients)
INSERT INTO users (role, data) VALUES
  ('patient', '{"email": "aliyaa@therahand.com","password": "$2b$10$nWqp5DEYMSYu19RPiSUw/eVzmX4n2LFjwebz6k3/NqIEFO1GoQisW","name": "Aliyaa"}'),
  ('patient', '{"email": "ethan@therahand.com","password": "$2b$10$r6UlTChvWYWrYqzzYzOf4uB50iiXuTWU//WbDmy8zCfZ36Kl2qPLy","name": "Ethan"}'),
  ('patient', '{"email": "jhovanny@therahand.com","password": "$2b$10$4NMy.B8/rmGdvm5j1EDlQemP.c9fQDW8NWTaCZE9ViXr07.inJcvO","name": "Jhovanny"}'),
  ('patient', '{"email": "a@abc.com","password": "$2b$10$M11TSOnb7LQGSAElX7HAo.g/upzTbAp5U5A9hUMK9KTxj07JUzzau","name": "User A"}');

-- Promote those users to patients
INSERT INTO patients (id)
SELECT id FROM users WHERE role = 'patient';

-- Insert users (doctors)
INSERT INTO users (role, data) VALUES
  ('doctor', '{"email": "dr.harrison@therahand.com","password": "$2b$10$3xm.L4KzlLj25ISSPKtYG.LtTucGZKyFQ1WyakPzqJ5FEdVPf80QC","name": "Dr. Harrison"}'),
  ('doctor', '{"email": "dr.lu@therahand.com","password": "$2b$10$WHaord9QWJQD2wr2BdY6ROSw21w6LF5tPiwoA7u67PtQKE.XuEnOi","name": "Dr. Lu"}');

-- Promote those users to doctors
INSERT INTO doctors (id)
SELECT id FROM users WHERE role = 'doctor';

-- Insert users (admins)
INSERT INTO users (role, data) VALUES
  ('admin', '{"email": "a@admin.com","password": "$2b$10$VYomD07ETgcZ2O3Uj7Tcheiv4y.F0Yj3aAEBCm7qTcfg6y/wtXA/a","name": "Admin"}');

-- Promote those users to admins
INSERT INTO admins (id)
SELECT id FROM users WHERE role = 'admin';

-- Assign doctor-patient relationships
INSERT INTO doctor_patients (doctor_id, patient_id) VALUES
  (
    (SELECT id FROM users WHERE data->>'email' = 'dr.harrison@therahand.com'),
    (SELECT id FROM users WHERE data->>'email' = 'aliyaa@therahand.com')
  ),
  (
    (SELECT id FROM users WHERE data->>'email' = 'dr.harrison@therahand.com'),
    (SELECT id FROM users WHERE data->>'email' = 'ethan@therahand.com')
  ),
  (
    (SELECT id FROM users WHERE data->>'email' = 'dr.lu@therahand.com'),
    (SELECT id FROM users WHERE data->>'email' = 'jhovanny@therahand.com')
  );

-- Insert fluff messages
INSERT INTO messages (sender_id, recipient_id, content, sent_at) VALUES
  -- dr. harrison to Aliyaa
  ((SELECT id FROM users WHERE data->>'email' = 'dr.harrison@therahand.com'),
   (SELECT id FROM users WHERE data->>'email' = 'aliyaa@therahand.com'),
   'Hello Aliyaa, how is your therapy going this week?', '2024-12-02 14:30:00'),

  -- Aliyaa to dr. harrison
  ((SELECT id FROM users WHERE data->>'email' = 'aliyaa@therahand.com'),
   (SELECT id FROM users WHERE data->>'email' = 'dr.harrison@therahand.com'),
   'Hi Dr. Harrison! I''m feeling better, thanks for checking in.', '2024-12-02 15:30:00'),

  -- Dr. Lu to Jhovanny
  ((SELECT id FROM users WHERE data->>'email' = 'dr.lu@therahand.com'),
   (SELECT id FROM users WHERE data->>'email' = 'jhovanny@therahand.com'),
   'Jhovanny, please upload your finger movement logs.', '2024-12-03 14:30:00'),

  -- Jhovanny to Dr. Lu
  ((SELECT id FROM users WHERE data->>'email' = 'jhovanny@therahand.com'),
   (SELECT id FROM users WHERE data->>'email' = 'dr.lu@therahand.com'),
   'Sure, I''ll do that by tonight.', '2024-12-03 15:30:00'),

  -- Ethan to dr. harrison
  ((SELECT id FROM users WHERE data->>'email' = 'ethan@therahand.com'),
   (SELECT id FROM users WHERE data->>'email' = 'dr.harrison@therahand.com'),
   'Hi Doctor, my index finger still hurts a little. Any tips?', '2024-12-04 14:30:00');
