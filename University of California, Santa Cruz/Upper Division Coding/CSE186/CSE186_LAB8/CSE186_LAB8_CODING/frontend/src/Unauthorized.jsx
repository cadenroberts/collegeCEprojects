import {useNavigate} from 'react-router-dom';
import Button from '@mui/material/Button';

const Unauthorized = () => {
  const navigate = useNavigate();
  return (
    <div style={{textAlign: 'center', marginTop: '50px'}}>
      <h1>Unauthorized Access</h1>
      <h2>You do not have access to this page.</h2>
      <Button margintop='5000px' variant="contained"
        onClick={() => navigate('/')}
        color="secondary" role="button" aria-label="backToLogin">
        Back to Login
      </Button>
    </div>
  );
};

export default Unauthorized;
