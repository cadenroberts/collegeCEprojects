\connect postgres
DROP TABLE IF EXISTS doctor_patients CASCADE;
DROP TABLE IF EXISTS patient_devices CASCADE;
DROP TABLE IF EXISTS messages CASCADE;
DROP TABLE IF EXISTS doctors CASCADE;
DROP TABLE IF EXISTS patients CASCADE;
DROP TABLE IF EXISTS admins CASCADE;
DROP TABLE IF EXISTS users CASCADE;
DROP TABLE IF EXISTS devices CASCADE;
DROP TABLE IF EXISTS runner          CASCADE;
DROP TABLE IF EXISTS results         CASCADE;
DROP TABLE IF EXISTS assignments         CASCADE;

CREATE EXTENSION IF NOT EXISTS "pgcrypto";

CREATE TABLE users (
  id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
  role TEXT CHECK (role IN ('doctor', 'patient', 'admin')) NOT NULL,
  data JSONB
);

CREATE TABLE patients (
  id UUID PRIMARY KEY REFERENCES users(id) ON DELETE CASCADE
);

CREATE TABLE doctors (
  id UUID PRIMARY KEY REFERENCES users(id) ON DELETE CASCADE
);

CREATE TABLE admins (
  id UUID PRIMARY KEY REFERENCES users(id) ON DELETE CASCADE
);

CREATE TABLE doctor_patients (
  doctor_id UUID NOT NULL REFERENCES doctors(id) ON DELETE CASCADE,
  patient_id UUID NOT NULL REFERENCES patients(id) ON DELETE CASCADE,
  PRIMARY KEY (doctor_id, patient_id)
);

CREATE TABLE messages (
  id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
  sender_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
  recipient_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
  content TEXT NOT NULL,
  sent_at TIMESTAMP DEFAULT (CURRENT_TIMESTAMP AT TIME ZONE 'America/Los_Angeles')
);

CREATE TABLE devices (
  id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
  hardware_id TEXT NOT NULL
);

CREATE TABLE patient_devices (
  patient_id UUID NOT NULL REFERENCES patients(id) ON DELETE CASCADE,
  device_id  UUID NOT NULL REFERENCES devices(id) ON DELETE CASCADE,
  PRIMARY KEY (patient_id, device_id)
);

CREATE OR REPLACE FUNCTION validate_message_permissions()
RETURNS TRIGGER AS $$
DECLARE
  sender_role TEXT;
  recipient_role TEXT;
  is_assigned BOOLEAN;
BEGIN
  SELECT role INTO sender_role FROM users WHERE id = NEW.sender_id;
  SELECT role INTO recipient_role FROM users WHERE id = NEW.recipient_id;

  IF sender_role = 'doctor' AND recipient_role = 'patient' THEN
    RETURN NEW;
  END IF;

  IF sender_role = 'patient' AND recipient_role = 'doctor' THEN
    SELECT EXISTS (
      SELECT 1 FROM doctor_patients
      WHERE doctor_id = NEW.recipient_id
        AND patient_id = NEW.sender_id
    ) INTO is_assigned;

    IF is_assigned THEN
      RETURN NEW;
    ELSE
      RAISE EXCEPTION 'Patient is not assigned to this doctor';
    END IF;
  END IF;

  RAISE EXCEPTION 'Invalid message direction from % to %', sender_role, recipient_role;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER enforce_message_permissions
  BEFORE INSERT ON messages
  FOR EACH ROW
  EXECUTE FUNCTION validate_message_permissions();

-- EXERCISES AND SESSIONS FOR EACH PATIENT --

CREATE TABLE results (
  id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
  device_id UUID NOT NULL REFERENCES devices(id) ON DELETE CASCADE,
  data JSONB NOT NULL,
  created_at TIMESTAMP DEFAULT (CURRENT_TIMESTAMP AT TIME ZONE 'America/Los_Angeles')
);

CREATE TABLE assignments (
  id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
  patient_id UUID NOT NULL REFERENCES patients(id) ON DELETE CASCADE,
  data JSONB NOT NULL
);

CREATE TABLE runner (
  id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
  device_id UUID NOT NULL REFERENCES devices(id) ON DELETE CASCADE,
  data JSONB NOT NULL
);

