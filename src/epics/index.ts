import { Action, AnyAction } from 'redux';
import {
    ActionsObservable,
    combineEpics,
    createEpicMiddleware,
    EpicMiddleware,
    StateObservable,
} from 'redux-observable';

import { BehaviorSubject, Observable, EMPTY } from 'rxjs';
import io from 'socket.io-client';
import { VariantType } from 'notistack';
import { StoreState } from '../reducers';

import counters from './Counters';
import webSocket from './webSocket';
import chat from './chat';

export type Socket = typeof io.Socket;

interface CustomError extends Error {
    message: string;
    type?: VariantType;
}

export const customError = (error: object) => {
    const err = new Error(error['message']);
    err['type'] = error['type'];
    return err as CustomError;
};

export const errHandler = ({ message, type }: CustomError, customAction?: AnyAction) => {
    return EMPTY;
};

const dependencies = { io, socket$: new BehaviorSubject((null as unknown) as Socket), sessionStorage };

export type Dependencies = typeof dependencies;

export type Epic<T extends Action = Action> = (
    action$: ActionsObservable<Action>,
    state$: StateObservable<StoreState>,
    dependencies: Dependencies,
) => Observable<AnyAction>;

export const epicMiddleware: EpicMiddleware<Action, Action, StoreState> = createEpicMiddleware({
    dependencies,
});

export const epics = combineEpics(...counters, ...webSocket, ...chat);
