import React, { FC, memo } from 'react';

import Progress from '../Progress';

import AppBar from '../../containers/AppBar';
import { Props } from '../../containers/Frame';

import useStyles from '../../styles/frame';

const Frame: FC<Props> = memo(({ children, userInfo, loading }) => {
    const classes = useStyles();

    return (
        <div className={classes.root}>
            <AppBar />
            <main className={classes.content}>{userInfo && children}</main>
            {loading && <Progress />}
        </div>
    );
});

export default Frame;
