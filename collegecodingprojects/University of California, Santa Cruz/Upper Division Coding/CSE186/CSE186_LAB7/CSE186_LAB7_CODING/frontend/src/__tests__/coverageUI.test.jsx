import {it, beforeAll, afterAll, afterEach} from 'vitest';
import {render, fireEvent, screen} from '@testing-library/react';
import App from '../App';
import {setupServer} from 'msw/node';
import {http, HttpResponse} from 'msw';

const server = setupServer();

/**
 * @param {string} pattern
 * @return {element} clickable
 */
export function getClickable(pattern) {
  return screen.getByRole('button', {
    name: pattern,
  });
}


beforeAll(() => server.listen());

// Reset any runtime request handlers we may add during the tests
afterEach(() => server.resetHandlers());

// Clean up once the tests are done
afterAll(() => server.close());

it('Coverage', async () => {
  server.use(
      http.get('http://localhost:3010/v0/mail', (req) => {
        const mailbox = new URL(req.request.url).searchParams.get('mailbox');
        let response;
        switch (mailbox) {
          case 'inbox':
            response = [
              {
                id: '05ca5335-8948-4973-8d86-d0358acde5cd',
                to: {
                  name: 'CSE186 Student',
                  email: 'CSE186@ucsc.edu',
                },
                from: {
                  name: 'Shirleen Chin',
                  email: 'schin2m@ca.gov',
                },
                received: '2019-11-24T04:35:03Z',
                sent: '2020-03-08T13:31:42Z',
                subject: 'Compatible client-server alliance',
              },
            ];
            break;
          case 'sent':
            response = [
              {
                id: '9f626384-8b75-4a4d-8d25-05a86f05938e',
                to: {
                  name: 'CSE186 Student',
                  email: 'CSE186t@ucsc.edu',
                },
                from: {
                  name: 'Giacinta Padley',
                  email: 'ga@xinnet.com',
                },
                received: '2020-09-07T09:15:32Z',
                sent: '2020-01-06T01:27:34Z',
                subject: 'Intuitive 4th generation concept',
              },
            ];
            break;
          case 'trash':
            response = [
              {
                id: 'd3dd8c57-41ad-4b30-8867-40a6e8485972',
                to: {
                  name: 'CSE186 Student',
                  email: 'CSE186@ucsc.edu',
                },
                from: {
                  name: 'Randolph Inchley',
                  email: 'rileyr@de.vu',
                },
                received: '2020-08-21T13:20:52Z',
                sent: '2020-07-14T16:26:46Z',
                subject: 'Synchronised maximized approach',
              },
            ];
            break;
        }
        return HttpResponse.json(response, {status: 200});
      }),
      http.put(
          'http://localhost:3010/v0/mail/:id', (req) => {
            const mailbox = new URL(
                req.request.url).searchParams.get('mailbox');
            if (mailbox === 'trash') {
              return HttpResponse.json({status: 204});
            }
          }),
  );

  const today = new Date().toISOString();
  const initial = [
    {
      id: '148363d5-d951-4f68-a526-06346a885c71',
      to: {
        name: 'CSE186 Student',
        email: 'CSE186student@ucsc.edu',
      },
      from: {
        name: 'Nessie Doogood',
        email: 'ndoogood0@clickbank.net',
      },
      received: '2020-09-14T14:48:28Z',
      sent: '2020-09-02T09:50:17Z',
      subject: 'Integrated optimizing moderator',
    },
    {
      id: '89014143-6f72-41bb-a51f-a725475dfa08',
      to: {
        name: 'CSE186 Student',
        email: 'CSE186student@ucsc.edu',
      },
      from: {
        name: 'Constantine Withrop',
        email: 'cwithrop1@cmu.edu',
      },
      received: '2020-03-31T10:25:38Z',
      sent: '2020-03-03T08:03:11Z',
      subject: 'Universal contextually-based focus group',
    },
    {
      id: '28545519-ca73-464c-9e71-faca98388327',
      to: {
        name: 'CSE186 Student',
        email: 'CSE186student@ucsc.edu',
      },
      from: {
        name: 'Brandea Linnard',
        email: 'blinnard2@devhub.com',
      },
      received: '2024-06-30T13:10:25Z',
      sent: '2024-05-22T13:04:25Z',
      subject: 'Decentralized real-time secured line',
    },
    {
      id: 'ebdd8995-a28b-4189-b16a-94c230a7eb91',
      to: {
        name: 'CSE186 Student',
        email: 'CSE186student@ucsc.edu',
      },
      from: {
        name: 'Tabbatha Gurry',
        email: 'tgurry3@europa.eu',
      },
      received: today,
      sent: today,
      subject: 'Pre-emptive disintermediate toolset',
    },
  ];
  render(<App initialEmails={initial}/>);
  fireEvent.click(screen.getByText('Trash'));
  fireEvent.click(screen.getByText('Sent'));
  fireEvent.click(screen.getByText('Inbox'));
  fireEvent.click(getClickable(
      'Delete mail from Constantine Withrop received 2020'));
});

