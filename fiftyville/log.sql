-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Have a look at what we have from reports on July 28 Humphrey st. Can specify the day.

SELECT *
FROM crime_scene_reports
WHERE day = 28 AND month = 7;
-- Result gave us details: Time was 10:15 and place was Bakery; ID = 295. Let's see what we have from interviews exactly;

SELECT *
FROM interviews
WHERE day = 28 AND month = 7 AND transcript LIKE "%bakery%";
-- There a few transactions BUT as we were told all of them should mention bakery. So we left with just these:
-- First:
--| 161 | Ruth    | 2023 | 7     | 28  | Sometime within ten minutes of the theft,
-- I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Second:
-- | 162 | Eugene  | 2023 | 7     | 28  | I don't know the thief's name, but it was someone I recognized.
-- Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- Third:
-- | 163 | Raymond | 2023 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |

-- Let's see who withdraw money in the morning on ATM on Leggett Street;
SELECT *
FROM atm_transactions
WHERE day = 28 AND month = 7 AND transaction_type = "withdraw" AND atm_location = "Leggett Street";
-- Okey, we do have few results, but not clear yet wich one is ours. Let's gather more information and come back later to this.

-- Let's check parking logs
SELECT *
FROM bakery_security_logs
WHERE year = 2023 AND DAY = 28 AND month = 7 AND hour = 10 AND minute < 30 AND minute > 20 AND activity = "exit";
-- Okey, we do have few results, but not clear yet wich one is ours. Let's gather more information and come back later to this.

-- Let's check phone calls. We now that Call was less than minute, Flight tomorrow and partner need to buy flight ticket.
SELECT *
FROM phone_calls WHERE year = 2023 AND DAY = 28 AND month = 7 AND duration < 60;
-- We do have a few results again...

-- Let's ger person_id from account number that we have
SELECT person_id
FROM bank_accounts
WHERE account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE day = 28 AND month = 7 AND transaction_type = "withdraw" AND atm_location = "Leggett Street"
    );


-- So we have list of callers, license_plate and account_numbers gave us ids. We need to find a match now. Let's try check people table (for caller and receiver).
-- --Let's create suspect table
CREATE TABLE suspects (
    id INTEGER,
    name TEXT,
    phone_number TEXT,
    passport_number TEXT,
    license_plate TEXT
);
INSERT INTO suspects
SELECT *
FROM people
WHERE phone_number IN (
        SELECT caller
        FROM phone_calls
        WHERE year = 2023 AND DAY = 28 AND month = 7 AND duration < 60
    )
  AND license_plate IN (
        SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2023 AND DAY = 28 AND month = 7 AND hour = 10 AND minute < 25 AND minute > 15 AND activity = 'exit'
    )
  AND id IN (
        SELECT person_id
        FROM bank_accounts
        WHERE account_number IN (
            SELECT account_number
            FROM atm_transactions
            WHERE day = 28 AND month = 7 AND transaction_type = 'withdraw' AND atm_location = 'Leggett Street'
        )
    );
-- We have 2 suspects left
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
-- +--------+-------+----------------+-----------------+---------------+


--Let's where would go earliest flight tomorrow from Fiftyville

SELECT *
FROM flights
WHERE origin_airport_id IN (SELECT id FROM airports WHERE city = "Fiftyville") AND day = 29 AND year = 2023 AND month = 7 ORDER BY hour,minute LIMIT 1;
--| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
--+----+-------------------+------------------------+------+-------+-----+------+--------+
--| 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     |
--+----+-------------------+------------------------+------+-------+-----+------+--------+

-- Let's check who who from ou suspects is a match
SELECT name FROM suspects WHERE passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = 36);
-- BRUCE!

--What city he went
SELECT city FROM airports WHERE id = 4;
--New York City




-- Who helped him.
SELECT receiver
FROM phone_calls
WHERE year = 2023 AND DAY = 28 AND month = 7 AND duration < 60 AND caller = "(367) 555-5533";
-- Receiver number is (375) 555-8161
SELECT *
FROM people
WHERE phone_number = "(375) 555-8161";
-- And we have helper
--+--------+-------+----------------+-----------------+---------------+
--|   id   | name  |  phone_number  | passport_number | license_plate |
--+--------+-------+----------------+-----------------+---------------+
--| 864400 | Robin | (375) 555-8161 | NULL            | 4V16VO0       |
