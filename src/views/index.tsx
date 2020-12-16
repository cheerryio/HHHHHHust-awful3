import React, { FC, lazy, memo, Suspense } from 'react';
import { Route, Switch, withRouter, RouteComponentProps, Redirect } from 'react-router';
import SnackBar from '../components/SnackBar';
import withRoot from '../styles/withRoot';
import Progress from '../components/Progress';
import Frame from '../containers/Frame';
const Game = lazy(() => import('./game'));
const Index: FC = memo(() => {
    const routeRender = (Component: JSX.Element) => (props: RouteComponentProps) => (
        <Frame {...props}>
            <Suspense fallback={<Progress />}>{Component}</Suspense>
        </Frame>
    );

    const RedirectToGame: FC = memo(() => <Redirect to='/game' />);

    return (
        <React.Fragment>
            <SnackBar>
                <Switch>
                    <Route path='/game' render={routeRender(<Game />)} />
                    <Route path='/' component={RedirectToGame} />
                </Switch>
            </SnackBar>
        </React.Fragment>
    );
});

export default withRouter(withRoot(Index));
