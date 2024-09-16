import {useContext} from 'react';
import {EmailContext} from './EmailContext.jsx';
import AppBar from '@mui/material/AppBar';
import Box from '@mui/material/Box';
import CssBaseline from '@mui/material/CssBaseline';
import Divider from '@mui/material/Divider';
import Drawer from '@mui/material/Drawer';
import IconButton from '@mui/material/IconButton';
import InboxIcon from '@mui/icons-material/MoveToInbox';
import List from '@mui/material/List';
import ListItem from '@mui/material/ListItem';
import ListItemButton from '@mui/material/ListItemButton';
import ListItemIcon from '@mui/material/ListItemIcon';
import MailIcon from '@mui/icons-material/Mail';
import DeleteIcon from '@mui/icons-material/Delete';
import Toolbar from '@mui/material/Toolbar';
import Typography from '@mui/material/Typography';
import {
  Table, TableBody, TableCell,
  TableContainer, TableRow, Paper,
} from '@mui/material';


// https://mui.com/material-ui/react-drawer/ RESPONSIVE DRAWER
// chatgpt.com & my brain worked together with responsive drawer
// as the base for this assignment.

const drawerWidth = 240;

/**
 * @param {object} props - The props passed to the component.
 * @param {function} props.window - Function to return the window object.
 * @param {array} props.emails - Array of email objects to display
 * @return {JSX.Element} The rendered ResponsiveDrawer component.
 */
function ResponsiveDrawer() {
  const {emails, selectedMailbox, setSelectedMailbox,
    handleTrashMove} = useContext(EmailContext);

  // if (initial.length) setEmails(initial);

  const handleMailboxClick = (mailbox) => {
    setSelectedMailbox(mailbox);
  };

  const sortEmails = (emails) => {
    return emails.sort((a, b) => new Date(b.received) - new Date(a.received));
  };

  const formatDate = (date) => {
    const subMonths = (date, months) => {
      const result = new Date(date);
      result.setMonth(result.getMonth() - months);
      return result;
    };

    const parsedDate = new Date(date);
    const twelveMonthsAgo = subMonths(new Date('2024-01-01T00:00:00'), 12);

    const isToday = (date) => {
      const today = new Date();
      return date.getDate() === today.getDate() &&
             date.getMonth() === today.getMonth() &&
             date.getFullYear() === today.getFullYear();
    };

    const isAfter = (date1, date2) => date1 > date2;

    if (isToday(parsedDate)) {
      return parsedDate.toLocaleTimeString(
          'en-GB', {hour: '2-digit', minute: '2-digit'});
    } else if (isAfter(parsedDate, twelveMonthsAgo)) {
      return parsedDate.toLocaleDateString(
          'en-US', {day: '2-digit', month: 'short'});
    } else {
      return parsedDate.getFullYear().toString();
    }
  };

  const drawer = (
    <div>
      <Toolbar />
      <Divider />
      <List>
        <ListItem key={'inbox'} disablePadding>
          <ListItemButton onClick={() => handleMailboxClick('inbox')}
            aria-label = "Select Inbox" data-testid="inbox-button">
            <ListItemIcon>
              <InboxIcon />
            </ListItemIcon>
            Inbox
          </ListItemButton>
        </ListItem>
      </List>
      <Divider />
      <List>
        <ListItem key={'sent'} disablePadding>
          <ListItemButton onClick={() => handleMailboxClick('sent')}
            aria-label = "Select Sent" data-testid="sent-button">
            <ListItemIcon>
              <MailIcon />
            </ListItemIcon>
            Sent
          </ListItemButton>
        </ListItem>
      </List>
      <Divider />
      <List>
        <ListItem key={'trash'} disablePadding>
          <ListItemButton onClick={() => handleMailboxClick('trash')}
            aria-label = "Select Trash" data-testid="trash-button">
            <ListItemIcon>
              <DeleteIcon />
            </ListItemIcon>
            Trash
          </ListItemButton>
        </ListItem>
      </List>
    </div>
  );

  return (
    <Box sx={{display: 'flex'}}>
      <CssBaseline />
      <AppBar
        position="fixed"
        sx={{zIndex: (theme) => theme.zIndex.drawer + 1}}
      >
        <Toolbar>
          <Typography variant="h6" noWrap component="div"
            data-testid="appbar-title" color="inherit">
            <IconButton aria-label="title toggle"
              sx={{color: 'inherit'}}>
              CSE186 Full Stack Mail - {selectedMailbox.charAt(0).toUpperCase()+
                selectedMailbox.slice(1)}
            </IconButton>
          </Typography>
        </Toolbar>
      </AppBar>
      <Box
        component="nav"
        sx={{width: {sm: drawerWidth}, flexShrink: {sm: 0}}}
        aria-label="mailbox folders"
      >
        <Drawer
          variant="permanent"
          sx={{
            'display': {sm: 'block'},
            '& .MuiDrawer-paper': {boxSizing: 'border-box', width: drawerWidth},
          }}
          open
        >
          {drawer}
        </Drawer>
      </Box>
      <Box
        component="main"
        sx={{flexGrow: 1, p: 3, width: {sm: `calc(100% - ${drawerWidth}px)`}}}
      >
        <Toolbar />
        <TableContainer component={Paper}>
          <Table>
            <TableBody>
              {sortEmails(emails).map((email) => (
                <TableRow key={email.id}
                  style={{cursor: 'pointer'}} role="button"
                  aria-label={`${email.from.name} ${email.subject}`}>
                  <TableCell>
                    {selectedMailbox !== 'trash' ?
                    (<IconButton edge="end"
                      color="gray" onClick={() => handleTrashMove(email)}
                      role="button"
                      aria-label= {
                        'Delete mail' + (selectedMailbox === 'inbox' ?
                        ' from ' : ' to ') + (selectedMailbox === 'inbox' ?
                        email.from.name : email.to.name) + (selectedMailbox ===
                        'inbox' ? ' received ' : ' sent ') +
                        formatDate(email.received)}
                      style={{display: 'block'}}>
                      <DeleteIcon />
                    </IconButton>
                    ) : null}
                  </TableCell>
                  <TableCell>
                    {selectedMailbox === 'inbox' ? email.from.name :
                    (selectedMailbox === 'sent' ? email.to.name :
                    email.from.name+' to '+email.to.name)}
                    <br />
                    <span style={{color: 'gray'}}>
                      {email.subject}
                    </span>
                  </TableCell>
                  <TableCell>
                    {formatDate(email.received)}
                  </TableCell>
                </TableRow>
              ))}
            </TableBody>
          </Table>
        </TableContainer>
      </Box>
    </Box>
  );
}

export default ResponsiveDrawer;
