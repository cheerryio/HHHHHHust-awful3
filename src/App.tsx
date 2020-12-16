import { createBrowserHistory as createHistory } from 'history';
import React, { FC, memo } from 'react';
import './App.css';
import { applyMiddleware, createStore } from 'redux';
import { Provider } from 'react-redux';
import { reducers } from './reducers';
import { BrowserRouter as Router } from 'react-router-dom';
import { composeWithDevTools } from 'redux-devtools-extension';
import Index from './views';
import { epicMiddleware, epics } from './epics';
createHistory();

const middleware = [epicMiddleware];

export const store = createStore(reducers, composeWithDevTools(applyMiddleware(...middleware)));

epicMiddleware.run(epics);

const App: FC = memo(() => {
    return (
        <Provider store={store}>
            <Router>
                <Index />
            </Router>
        </Provider>
    );
});

export default App;
