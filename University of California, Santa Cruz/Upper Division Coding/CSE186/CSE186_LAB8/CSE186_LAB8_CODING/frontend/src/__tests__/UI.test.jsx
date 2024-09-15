import {it, beforeAll, afterAll, afterEach, expect} from 'vitest';
import {render, fireEvent, screen, waitFor} from '@testing-library/react';
import App from '../App';
import {setupServer} from 'msw/node';
import {http, HttpResponse} from 'msw';
import {MemoryRouter} from 'react-router-dom';

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

beforeAll(() => {
  server.listen();
  window.resizeTo = function resizeTo(width, height) {
    Object.assign(this, {
      innerWidth: width,
      innerHeight: height,
      outerWidth: width,
      outerHeight: height,
    }).dispatchEvent(new this.Event('resize'));
  };
});

// Reset any runtime request handlers we may add during the tests
afterEach(() => server.resetHandlers());

// Clean up once the tests are done
afterAll(() => server.close());

it('Login', async () => {
  server.use(
      http.post('http://localhost:3010/v0/', (req) => {
        const response = {'name': 'TesterA',
          'accessToken': 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlbWFpbCI6ImF'+
          'AYWJjLmNvbSIsInJvbGUiOiJtZW1iZXIiLCJpYXQiOjE3MjE2MzQ3MDUsImV4cCI6MT'+
          'cyMTYzNjUwNX0.m5CABmVqFL3mahmWdTjagfRWRZRQwhX3B0ajQMS_amU'};
        return HttpResponse.json(response, {status: 200});
      }),
  );
  const {unmount} = render(
      <App router={MemoryRouter} initialEntries={['/home']}/>);
  unmount();
  render(<App router={MemoryRouter} initialEntries={['/']}/>);
  fireEvent.click(getClickable('Login'));
  fireEvent.click(getClickable('rememberMe'));
  fireEvent.change(screen.getByLabelText(/Email Address/i), {
    target: {value: 'a@abc.com'},
  });
  fireEvent.change(screen.getByLabelText(/Password/i), {
    target: {value: 'a'},
  });
  fireEvent.click(getClickable('Login'));
  fireEvent.click(getClickable('rememberMe'));
  fireEvent.change(screen.getByLabelText(/Email Address/i), {
    target: {value: 'a@abc.com'},
  });
  fireEvent.change(screen.getByLabelText(/Password/i), {
    target: {value: 'a'},
  });
  fireEvent.click(getClickable('Login'));
  localStorage.setItem('remember', JSON.stringify({email: 'a@abc.com',
    password: 'a'}));
  render(<App />);
});

it('Home', async () => {
  window.resizeTo(550, 1024);
  server.use(
      http.post('http://localhost:3010/v0/', (req) => {
        const response = {
          'id': '2547b98a-4d62-4e93-b789-c2eed30bdcbf',
          'accessToken': 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjI1ND'+
            'diOThhLTRkNjItNGU5My1iNzg5LWMyZWVkMzBiZGNiZiIsImlhdCI6MTcyMTk3MD'+
            'I4NSwiZXhwIjoxNzIxOTcyMDg1fQ.p-5CFvzgSlZLneOyvlO5eDS9f_9778C_7sc'+
            'VG2-N9As',
        };
        return HttpResponse.json(response, {status: 200});
      }),
      http.get('http://localhost:3010/v0/home/workspaces', (req) => {
        const response = [
          {
            'id': '1dfeab9d-d938-4d5d-9d7b-07bec70a1ffd',
            'owner_id': '11d21d95-cf21-42af-b5b6-c4bca79b8bb6',
            'data': {
              'name': 'Molly Workspace',
            },
          },
          {
            'id': '5e2234cb-c152-413d-a456-f7b6ade5e46f',
            'owner_id': '48fe3ecf-6170-4c62-b69f-646eea4b73c6',
            'data': {
              'name': 'Anna Workspace',
            },
          },
          {
            'id': 'de263fc7-e2eb-45cc-9ec1-a526dd7ac5f1',
            'owner_id': '52f4a648-a368-4ce5-ae37-8fbcd0354eb3',
            'data': {
              'name': 'User A Workspace',
            },
          },
        ];
        return HttpResponse.json(response, {status: 200});
      }),
      http.get('http://localhost:3010/v0/home/1dfeab9d-d938-4d5d-9d7b-07bec70'+
        'a1ffd/channels', (req) => {
        const response = [
          {
            'id': '4bf53b70-89c6-4f79-a2ee-905926ef1422',
            'workspace_id': '1dfeab9d-d938-4d5d-9d7b-07bec70a1ffd',
            'data': {
              'name': 'Molly Books',
            },
          },
          {
            'id': '50b73d81-8f6b-4ced-9084-232f5d1a897c',
            'workspace_id': '1dfeab9d-d938-4d5d-9d7b-07bec70a1ffd',
            'data': {
              'name': 'Molly Magazines',
            },
          },
        ];
        return HttpResponse.json(response, {status: 200});
      }),
      http.get('http://localhost:3010/v0/home/4bf53b70-89c6-4f79-a2ee-905926e'+
        'f1422/messages', (req) => {
        const response = [
          {
            'id': '61c498fd-90b7-42ce-a5bb-d30714c87c20',
            'channel_id': '4bf53b70-89c6-4f79-a2ee-905926ef1422',
            'data': {
              'date': '2024-06-13T14:15:19Z',
              'from': 'User A',
              'content': 'I miss reading.',
            },
          },
          {
            'id': 'f8d0c7ba-74ef-4aa2-a20d-1c8e39359f85',
            'channel_id': '4bf53b70-89c6-4f79-a2ee-905926ef1422',
            'data': {
              'date': '2024-06-13T14:16:19Z',
              'from': 'Anna Admin',
              'content': 'My favorite book is Where the Red Fern Grows.',
            },
          },
          {
            'id': '035daef9-29a9-4f66-a6b3-848389ef4e34',
            'channel_id': '4bf53b70-89c6-4f79-a2ee-905926ef1422',
            'data': {
              'date': '2024-06-14T14:16:19Z',
              'from': 'Molly Member',
              'content': 'Hey, does anyone love Harry Potter too?',
            },
          },
        ];
        return HttpResponse.json(response, {status: 200});
      }),
      http.get(`http://localhost:3010/v0/home/035daef9-29a9-4f66-a6b3-848389e'+
        'f4e34/threads`, (req) => {
      }),
  );
  const initialThreads = [
    {
      'id': '29393032-2838-403c-938e-e6906b95546f',
      'message_id': '035daef9-29a9-4f66-a6b3-848389ef4e34',
      'data': {
        'date': '2024-06-14T14:25:19Z',
        'from': 'Anna Admin',
        'content': 'The Chamber of Secrets is my favorite!',
      },
    },
    {
      'id': '787ac757-949e-4201-9f5f-3ab81860c8ce',
      'message_id': '035daef9-29a9-4f66-a6b3-848389ef4e34',
      'data': {
        'date': '2024-06-14T14:35:19Z',
        'from': 'User A',
        'content': 'Yes, though I do like the movies more than the book'+
          's, sue me!',
      },
    },
    {
      'id': '787ac757-949e-4201-9f5f-3ab92970c8ce',
      'message_id': '035daef9-29a9-4f66-a6b3-848389ef4e34',
      'data': {
        'date': '2024-06-20T14:35:19Z',
        'from': 'User A',
        'content': 'Yes, though I do like the books more than the movie'+
          's, sue me!',
      },
    },
  ];
  const today = new Date().toISOString();
  const initialMessages = [
    {
      'id': '61c498fd-90b7-42ce-a5bb-d30714c87c20',
      'channel_id': '4bf53b70-89c6-4f79-a2ee-905926ef1422',
      'data': {
        'date': today,
        'from': 'User A',
        'content': 'I miss reading.',
      },
    },
    {
      'id': 'f8d0c7ba-74ef-4aa2-a20d-1c8e39359f85',
      'channel_id': '4bf53b70-89c6-4f79-a2ee-905926ef1422',
      'data': {
        'date': '2022-06-13T14:16:19Z',
        'from': 'Anna Admin',
        'content': 'My favorite book is Where the Red Fern Grows.',
      },
    },
    {
      'id': 'f8d0c7ba-74ef-4aa2-a20d-1d8c49459e84',
      'channel_id': '4bf53b70-89c6-4f79-a2ee-905926ef1422',
      'data': {
        'date': '2022-06-13T14:16:19Z',
        'from': 'Anna Admin',
        'content': 'My favorite book is Where the Green Fern Grows.',
      },
    },
    {
      'id': '035daef9-29a9-4f66-a6b3-848389ef4e34',
      'channel_id': '4bf53b70-89c6-4f79-a2ee-905926ef1422',
      'data': {
        'date': '2024-06-14T14:16:19Z',
        'from': 'Molly Member',
        'content': 'Hey, does anyone love Harry Potter too?',
      },
    },
  ];
  render(<App router={MemoryRouter} initialEntries={['/unauthorized']}
    initialM={initialMessages} initialT={initialThreads}/>);
  fireEvent.click(getClickable('backToLogin'));
  fireEvent.change(screen.getByLabelText(/Email Address/i), {
    target: {value: 'a@abc.com'},
  });
  fireEvent.change(screen.getByLabelText(/Password/i), {
    target: {value: 'a'},
  });
  fireEvent.click(getClickable('Login'));
  await waitFor(() => {
    expect(localStorage.getItem('user')).toBeTruthy();
  });
  await waitFor(() => {
    window.history.pushState({}, 'Home', '/home');
  });
  fireEvent.click(getClickable('workspaceList'));
  fireEvent.click(getClickable('Molly Workspace'));
  fireEvent.click(getClickable('Channel Molly Books'));
  fireEvent.click(getClickable('home'));
  fireEvent.click(getClickable('Channel Molly Books'));
  fireEvent.click(getClickable('back'));
  fireEvent.click(getClickable('Channel Molly Books'));
  fireEvent.click(getClickable('Hey, does anyone love Harry Potter too?'));
  // wait for # of threads to appear screen.findByText
  fireEvent.click(getClickable('back'));
  fireEvent.click(getClickable('logout'));
});
