import React, { FC, memo } from 'react';
import Button from '@material-ui/core/Button';
import TextField from '@material-ui/core/TextField';
import Grid from '@material-ui/core/Grid';
import AccountCircle from '@material-ui/icons/AccountCircle';

import useStyles from '../../styles/login';

const Login: FC = memo(({}) => {
    const classes = useStyles();

    return (
        <React.Fragment>
            <div>
                <div className={classes.margin}>
                    <Grid container spacing={1} alignItems='flex-end'>
                        <Grid item>
                            <AccountCircle />
                        </Grid>
                        <Grid item>
                            <TextField id='account-name' label='With a grid' />
                        </Grid>
                    </Grid>
                </div>

                <Button variant='contained' color='primary'>
                    Login
                </Button>
            </div>
        </React.Fragment>
    );
});

export default Login;
