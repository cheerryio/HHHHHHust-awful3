import { ofType } from 'redux-observable';
import { Epic, Socket } from '../';
import { START_SOCKET } from '../../actions';
import { Observable } from 'rxjs';
import { switchMap, tap, ignoreElements } from 'rxjs/operators';
import { API } from '../../config/consts';

export const socketConnectEpic: Epic = (action$, state$, { io, socket$ }) =>
    action$.pipe(
        ofType(START_SOCKET),
        switchMap(
            () =>
                new Observable<Socket>((o) => {
                    const socket = io(API);
                    socket.on('connect', () => o.next(socket));
                    socket.on('disconnect', socket.close);
                }),
        ),
        tap(socket$),
        ignoreElements(),
    );

export default socketConnectEpic;
