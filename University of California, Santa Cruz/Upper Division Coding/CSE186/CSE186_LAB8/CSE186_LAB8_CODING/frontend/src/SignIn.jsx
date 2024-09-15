import Avatar from '@mui/material/Avatar';
import Button from '@mui/material/Button';
import CssBaseline from '@mui/material/CssBaseline';
import TextField from '@mui/material/TextField';
import FormControlLabel from '@mui/material/FormControlLabel';
import Checkbox from '@mui/material/Checkbox';

import Box from '@mui/material/Box';
import LockOutlinedIcon from '@mui/icons-material/LockOutlined';
import Typography from '@mui/material/Typography';
import Container from '@mui/material/Container';
import {createTheme, ThemeProvider} from '@mui/material/styles';

const defaultTheme = createTheme();
import {useNavigate} from 'react-router-dom';
import {useEffect, useContext} from 'react';
import {Context} from './Context.jsx';
/**
 * Sign in component
 *
 * @return {object} JSX
 */
export default function SignIn() {
  const {remember, setRemember, rememberEmail, setRememberEmail,
  } = useContext(Context);
  const history = useNavigate();
  const handleSubmit = async (event) => {
    event.preventDefault();
    const data = new FormData(event.currentTarget);
    const user = {
      email: data.get('email'),
      password: data.get('password'),
    };
    if (!user.email || !user.password) return;
    const response = await fetch('http://localhost:3010/v0/', {
      method: 'POST',
      body: JSON.stringify(user),
      headers: {
        'Content-Type': 'application/json',
      },
    });
    const json = await response.json();
    localStorage.setItem('user', JSON.stringify(json));
    if (remember) {
      localStorage.setItem('remember', JSON.stringify(user));
    } else {
      localStorage.removeItem('remember');
      setRememberEmail('');
    }
    history('/home');
  };
  const handleRememberChange = (event) => {
    if (remember === false) setRemember(true);
    else setRemember(false);
    // setRemember(!event.target.checked);
  };
  useEffect(() => {
    const storedUser = JSON.parse(localStorage.getItem('remember'));
    if (storedUser) {
      setRememberEmail(storedUser.email);
      setRemember(true);
    } else {
      setRememberEmail('');
      setRemember(false);
    }
  }, [setRemember, setRememberEmail]);
  return (
    <ThemeProvider theme={defaultTheme}>
      <Container component="main" maxWidth="xs">
        <CssBaseline />
        <Box
          sx={{
            marginTop: 8,
            display: 'flex',
            flexDirection: 'column',
            alignItems: 'center',
          }}
        >
          <Avatar sx={{m: 1, bgcolor: 'secondary.main'}}>
            <LockOutlinedIcon />
          </Avatar>
          <Typography component="h1" variant="h5" color="secondary">
            Sign in
          </Typography>
          <Box component="form" onSubmit={handleSubmit}
            noValidate sx={{mt: 1}} >
            <TextField
              margin="normal"
              required
              fullWidth
              id="email"
              label="Email Address"
              name="email"
              autoComplete="email"
              autoFocus
              onChange={(e) => setRememberEmail(e.target.value)}
              value = {rememberEmail}
              InputLabelProps={{
                sx: {
                  'color': 'darkorchid',
                  '&.Mui-focused': {
                    'color': 'darkorchid',
                  },
                  '&.MuiFormLabel-filled': {
                    'color': 'darkorchid',
                  },
                },
              }}
              sx={{
                '& label.Mui-focused': {
                  color: 'darkorchid',
                },
                '& .MuiOutlinedInput-root': {
                  '& fieldset': {
                    borderColor: 'darkorchid',
                  },
                  '&:hover fieldset': {
                    borderColor: 'darkorchid',
                  },
                  '&.Mui-focused fieldset': {
                    borderColor: 'darkorchid',
                  },
                  '& .MuiInputBase-input': {
                    color: 'darkorchid',
                  },
                },
              }}
            />
            <br/>
            <TextField
              margin="normal"
              required
              fullWidth
              name="password"
              label="Password"
              type="password"
              id="password"
              autoComplete="current-password"
              InputLabelProps={{
                sx: {
                  'color': 'darkorchid',
                  '&.Mui-focused': {
                    'color': 'darkorchid',
                  },
                  '&.MuiFormLabel-filled': {
                    'color': 'darkorchid',
                  },
                },
              }}
              sx={{
                '& label.Mui-focused': {
                  color: 'darkorchid',
                },
                '& .MuiOutlinedInput-root': {
                  '& fieldset': {
                    borderColor: 'darkorchid',
                  },
                  '&:hover fieldset': {
                    borderColor: 'darkorchid',
                  },
                  '&.Mui-focused fieldset': {
                    borderColor: 'darkorchid',
                  },
                  '& .MuiInputBase-input': {
                    color: 'darkorchid',
                  },
                },
              }}
            />
            <br/>
            <FormControlLabel
              role = 'button' aria-label='rememberMe'
              control={<Checkbox value="remember" color="secondary"
                onChange={handleRememberChange} checked={remember}
              />}
              label="Remember me"
              sx={{
                '& .MuiFormControlLabel-label': {
                  color: remember ? 'darkorchid' : 'grey',
                },
              }}
            />
            <br/>
            <Button
              type="submit"
              role='button'
              aria-label='Login'
              fullWidth
              variant="contained"
              color="secondary"
              sx={{mt: 3, mb: 2}}
            >
              Sign In
            </Button>
          </Box>
        </Box>
      </Container>
    </ThemeProvider>
  );
}
