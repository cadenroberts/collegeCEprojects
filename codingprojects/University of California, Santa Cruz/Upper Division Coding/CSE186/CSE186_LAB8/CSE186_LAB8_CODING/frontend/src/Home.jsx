import AppBar from '@mui/material/AppBar';
import Box from '@mui/material/Box';
import CssBaseline from '@mui/material/CssBaseline';
import Divider from '@mui/material/Divider';
import Drawer from '@mui/material/Drawer';
import List from '@mui/material/List';
import ListItem from '@mui/material/ListItem';
import ListItemButton from '@mui/material/ListItemButton';
import ListItemIcon from '@mui/material/ListItemIcon';
import ListItemText from '@mui/material/ListItemText';
import Toolbar from '@mui/material/Toolbar';
import Typography from '@mui/material/Typography';
import HomeIcon from '@mui/icons-material/Home';
import MessageIcon from '@mui/icons-material/Message';
import BottomNavigation from '@mui/material/BottomNavigation';
import BottomNavigationAction from '@mui/material/BottomNavigationAction';
import LogoutIcon from '@mui/icons-material/Logout';
import TagIcon from '@mui/icons-material/Tag';
import ArrowBackIosIcon from '@mui/icons-material/ArrowBackIos';
import Paper from '@mui/material/Paper';
import ExpandLess from '@mui/icons-material/ExpandLess';
import ExpandMore from '@mui/icons-material/ExpandMore';
import Collapse from '@mui/material/Collapse';
import {useNavigate} from 'react-router-dom';
import Grid from '@mui/material/Grid';
import {useState, useEffect} from 'react';
import {Context} from './Context.jsx';
import {useContext, useCallback} from 'react';

const drawerWidth = 240;

/**
 * @param {object} props - The props passed to the component.
 * @param {function} props.window - Function to return the window object.
 * @return {JSX.Element} The rendered ResponsiveDrawer component.
 */
function Home() {
  const {initialMessages, initialThreads} = useContext(Context);
  const history = useNavigate();
  const [channelDropdown, setChannelDropdown] = useState(false);
  const [workspaces, setWorkspaces] = useState([]);
  const [selectedWorkspace, setSelectedWorkspace] = useState('');
  const [channels, setChannels] = useState([]);
  const [selectedChannel, setSelectedChannel] = useState('');
  const [messages, setMessages] = useState(initialMessages);
  const [selectedMessage, setSelectedMessage] = useState('');
  const [threads, setThreads] = useState(initialThreads);
  let prevDate = '';
  const getWorkspaces = useCallback(async () => {
    const item = localStorage.getItem('user');
    const user = JSON.parse(item);
    const bearerToken = user.accessToken;
    fetch('http://localhost:3010/v0/home/workspaces', {
      method: 'GET',
      headers: {
        'Authorization': `Bearer ${bearerToken}`,
        'Content-Type': 'application/x-www-form-urlencoded',
      },
    })
        .then((response) => response.json())
        .then((workspaces) => {
          setWorkspaces(workspaces);
          setSelectedWorkspace(workspaces[0]);
          getChannels(workspaces[0].id);
        });
  }, []);
  const getChannels = async (id) => {
    const item = localStorage.getItem('user');
    const user = JSON.parse(item);
    const bearerToken = user.accessToken;
    fetch(`http://localhost:3010/v0/home/${id}/channels`, {
      method: 'GET',
      headers: {
        'Authorization': `Bearer ${bearerToken}`,
        'Content-Type': 'application/x-www-form-urlencoded',
      },
    })
        .then((response) => response.json())
        .then((channels) => {
          setChannels(channels);
        });
  };
  const getThreads = async (id) => {
    const item = localStorage.getItem('user');
    const user = JSON.parse(item);
    const bearerToken = user.accessToken;
    fetch(`http://localhost:3010/v0/home/${id}/threads`, {
      method: 'GET',
      headers: {
        'Authorization': `Bearer ${bearerToken}`,
        'Content-Type': 'application/x-www-form-urlencoded',
      },
    })
        .then((response) => response.json())
        .then((threads) => {
          setThreads(threads);
        });
  };

  useEffect(() => {
    getWorkspaces();
  }, [getWorkspaces]);
  useEffect(() => {
    if (selectedWorkspace) {
      getChannels(selectedWorkspace.id);
    }
  }, [selectedWorkspace]);
  useEffect(() => {
    if (selectedChannel) {
      getMessages(selectedChannel.id);
    }
  }, [selectedChannel]);
  useEffect(() => {
    if (selectedMessage) {
      getThreads(selectedMessage.id);
    }
  }, [selectedMessage]);
  const handleChannelDropdown = () => {
    setChannelDropdown(!channelDropdown);
  };
  const handleWorkspaceClick = (workspace) => {
    setSelectedWorkspace(workspace);
    setSelectedChannel('');
    setSelectedMessage('');
    handleChannelDropdown();
    getChannels(workspace.id);
  };
  const handleChannelClick = (channel) => {
    setSelectedChannel(channel);
    prevDate = '';
    getMessages(channel.id);
  };
  const handleMessageClick = (message) => {
    setSelectedMessage(message);
    prevDate = message.data.date;
    getThreads(message.id);
  };
  const getMessages = async (id) => {
    const item = localStorage.getItem('user');
    const user = JSON.parse(item);
    const bearerToken = user.accessToken;
    fetch(`http://localhost:3010/v0/home/${id}/messages`, {
      method: 'GET',
      headers: {
        'Authorization': `Bearer ${bearerToken}`,
        'Content-Type': 'application/x-www-form-urlencoded',
      },
    })
        .then((response) => response.json())
        .then((messages) => {
          setMessages(messages);
        });
  };
  const formatDate = (date) => {
    const subMonths = (date, months) => {
      const result = new Date(date);
      result.setMonth(result.getMonth() - months);
      return result;
    };
    const parsedDate = new Date(date);
    const twelveMonthsAgo = subMonths(new Date(), 12);
    const isToday = (date) => {
      const today = new Date();
      return date.getDate() === today.getDate() &&
             date.getMonth() === today.getMonth() &&
             date.getFullYear() === today.getFullYear();
    };

    const isAfter = (date1, date2) => date1 > date2;

    if (isToday(parsedDate)) {
      prevDate = parsedDate.toLocaleTimeString(
          'en-GB', {hour: '2-digit', minute: '2-digit'});
    } else if (isAfter(parsedDate, twelveMonthsAgo)) {
      prevDate = parsedDate.toLocaleDateString(
          'en-US', {day: '2-digit', month: 'short'});
    } else {
      prevDate = parsedDate.getFullYear().toString();
    }
    return prevDate;
  };
  const handleLogout = () => {
    localStorage.removeItem('user');
    history('/');
  };
  const handleBackButton = () => {
    if (selectedMessage) setSelectedMessage('');
    else if (selectedChannel) setSelectedChannel('');
  };
  const handleHomeButton = () => {
    setSelectedChannel('');
    setSelectedMessage('');
  };
  const drawer = (
    <div>
      <Toolbar key={'tool bars today 77'}/>
      <Divider key={'aNiceDivider'}/>
      <List key={'listKeyChannels'}>
        {channels.map((channel) => (
          <ListItem key={channel.id} disablePadding>
            <ListItemButton aria-label={'Channel '+channel.data.name}
              onClick={()=> handleChannelClick(channel)} role='button'
              key={'button '+channel.data.name}>
              <ListItemIcon key={channel.data.name+' icon'}>
                <TagIcon key={'tagIconKey '+channel.id}/>
              </ListItemIcon>
              <ListItemText primary={channel.data.name}
                key={'text '+channel.data.name}/>
            </ListItemButton>
          </ListItem>
        ))}
      </List>
      <Divider />
    </div>
  );

  return (
    <Box sx={{display: 'flex'}} key={'boxier boxy'}>
      <Box sx={{display: 'flex'}} key={'a boxxy box'}>
        <CssBaseline key={'css line base'}/>
        <AppBar
          position="fixed" key={'barapp999'}
          sx={{zIndex: (theme) => theme.zIndex.drawer + 1}}
          color="secondary"
        >
          <Toolbar sx={{display: 'flex'}} key={'toolBar 147'}>
            <Typography variant="h6" noWrap component="div">
              { selectedChannel ? (
                <Box sx={{display: 'flex', alignItems: 'center'}}
                  onClick={() => handleBackButton()} role='button'
                  aria-label='back' key={'messageBackBut'}>
                  <ArrowBackIosIcon key={'arrowBack'}
                    sx={{color: 'white', fontSize: 'small'}}/>
                  <ListItemText role='button' key={'backMesThr'}
                    primary={(selectedMessage ? 'Thread ' : '') +
                    selectedChannel.data.name} />
                </Box>
              ) : (
              <>
                <ListItemButton role='button'
                  onClick={handleChannelDropdown} aria-label='workspaceList'
                  key={'channelDropKey'}>
                  <ListItemText key={selectedWorkspace ?
                    'Listed '+selectedWorkspace.id : 'loading'}
                  primary={selectedWorkspace ?
                    selectedWorkspace.data.name : 'Loading...'} />
                  {channelDropdown ? <ExpandLess /> : <ExpandMore />}
                </ListItemButton>
                <Collapse in={channelDropdown} timeout="auto" unmountOnExit
                  key={'collapseWorkspaces'}>
                  <List component="div" disablePadding key={'work list'}>
                    {workspaces.map((workspace) => (
                      <ListItemButton sx={{pl: 4}} key={workspace.id}
                        onClick={() => handleWorkspaceClick(workspace)}
                        role='button' aria-label={workspace.data.name}>
                        <ListItemIcon key={'icon '+workspace.data.name}>
                          <MessageIcon sx={{color: 'white'}}
                            key={workspace.data.name+' micon'}/>
                        </ListItemIcon>
                        <ListItemText primary={workspace.data.name}
                          key={'text '+workspace.data.name}/>
                      </ListItemButton>
                    ))}
                  </List>
                </Collapse>
              </>
              )}
            </Typography>
          </Toolbar>
        </AppBar>
        {!selectedChannel ? (
        <Drawer
          variant="permanent" key={'channelDrawer'}
          sx={{
            'display': {xs: 'block'},
            '& .MuiDrawer-paper': {boxSizing: 'border-box',
              width: '100%'},
          }}
          open
        >
          {drawer}
        </Drawer>
        ) : null}
        <Box
          component="main" key={'boxmessage'}
          sx={{flexGrow: 1, p: 3, width: {sm:
            `calc(100% - ${drawerWidth}px)`}}}
        >
          <Toolbar key={'tool bar 123'} />
          <Grid container spacing={2} key={'messageGrid'}>
            {selectedMessage ? (
            <>
              <Divider sx={{width: '100%', my: 2}}/>
              <Grid item xs={12} key={'row selected ' + selectedMessage.id}>
                <Typography variant="h6"
                  key={'Selected header ' + selectedMessage.id}>
                  {formatDate(selectedMessage.data.date)}
                </Typography>
              </Grid>
              <Divider sx={{width: '100%'}}/>
              <Grid container item spacing={2}
                key={'Selected ' + selectedMessage.id}
                role="button" aria-label={selectedMessage.data.content}>
                <Grid item xs={3} key={'Selected date ' + selectedMessage.id}>
                  {(new Date(selectedMessage.data.date))
                      .toLocaleTimeString(
                          'en-GB', {hour: '2-digit', minute: '2-digit'})}
                </Grid>
                <Grid item xs={3} key={'Selected from ' + selectedMessage.id}>
                  {selectedMessage.data.from}
                </Grid>
                <Grid item xs={6}
                  key={'Selected content ' + selectedMessage.id}>
                  {selectedMessage.data.content}
                </Grid>
              </Grid>
              <Divider sx={{width: '100%', my: 2}}/>
              {threads.map((thread) => (
                <>
                  {prevDate !== formatDate(thread.data.date) ? (
                    <Grid item xs={12} key={'Date ' + thread.id}>
                      <Typography variant="subtitle1"
                        key={'Date cell ' + thread.id}>
                        {formatDate(thread.data.date)}
                      </Typography>
                    </Grid>
                  ) : null}
                  <Grid container item spacing={2} key={'Thread ' + thread.id}
                    role="button" aria-label={thread.data.content}>
                    <Grid item xs={3} key={'Thread date ' + thread.id}>
                      {(new Date(thread.data.date))
                          .toLocaleTimeString(
                              'en-GB', {hour: '2-digit', minute: '2-digit'})}
                    </Grid>
                    <Grid item xs={3} key={'Thread from ' + thread.id}>
                      {thread.data.from}
                    </Grid>
                    <Grid item xs={6} key={'Thread content ' + thread.id}>
                      {thread.data.content}
                    </Grid>
                  </Grid>
                </>
              ))}
            </>
          ) : (
            messages.map((message) => (
              <>
                {prevDate !== formatDate(message.data.date) ? (
                  <>
                    <Divider sx={{width: '100%', my: 2}}
                      key={'Divider1 '+message.id}/>
                    <Grid item xs={12} key={'Date ' + message.id}>
                      <Typography variant="h6" key={'Date cell ' + message.id}>
                        {formatDate(message.data.date)}
                      </Typography>
                    </Grid>
                    <Divider sx={{width: '100%'}} key={'Divider2 '+message.id}/>
                  </>
                ) : null}
                <Grid container item spacing={2} key={'Message ' + message.id}
                  style={{cursor: 'pointer'}} role="button"
                  onClick={() => handleMessageClick(message)}
                  aria-label={message.data.content}>
                  <Grid item xs={3} key={'Message date ' + message.id}>
                    {(new Date(message.data.date))
                        .toLocaleTimeString(
                            'en-GB', {hour: '2-digit', minute: '2-digit'})}
                  </Grid>
                  <Grid item xs={3} key={'Message from ' + message.id}>
                    {message.data.from}
                  </Grid>
                  <Grid item xs={6} key={'Message content ' + message.id}>
                    {message.data.content}
                  </Grid>
                </Grid>
              </>
            ))
          )}
          </Grid>
        </Box>
      </Box>
      <Paper sx={{position: 'fixed', bottom: 0, left: 0,
        right: 0, zIndex: (theme) => theme.zIndex.drawer + 1}} elevation={3}
      key={'paperbottomnav'}>
        <BottomNavigation sx={{width: 550}}
          value={''} key={'bottomnavbox'}>
          <BottomNavigationAction label="Home" value="home"
            icon={<HomeIcon />} disabled={!selectedChannel}
            onClick={() => handleHomeButton()} aria-label='home'
            role='button' key={'bottomhomenav'}
            sx={{color: selectedChannel ? 'darkorchid' : 'gainsboro',
              fontSize: 'large'}}
          />
          <BottomNavigationAction
            label="Logout"
            value="logout"
            icon={<LogoutIcon />}
            onClick={() => handleLogout()} aria-label='logout'
            role='button' key={'bottomnavlogout'}
            sx={{color: 'darkorchid', fontSize: 'large'}}
          />
        </BottomNavigation>
      </Paper>
    </Box>
  );
}

export default Home;
