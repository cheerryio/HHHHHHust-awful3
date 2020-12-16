import { Action } from 'redux';
import { ofType } from 'redux-observable';
import { Epic, errHandler } from '..';
import { SendMessage, SEND_MESSAGE, addMessageAction } from '../../actions';
import { switchMap, tap, map, catchError } from 'rxjs/operators';
import { EMPTY } from 'rxjs';

export const sendMessageEpic: Epic<SendMessage> = (action$, state$, { socket$ }) =>
    socket$.pipe(
        switchMap((socket) => {
            if (!socket) return EMPTY;
            return action$.pipe(
                ofType<Action, SendMessage>(SEND_MESSAGE),
                tap(({ message }: SendMessage) => {
                    socket.emit('message', { message });
                    console.log('emit message finish');
                }),
                map(({ message }: SendMessage) => {
                    return addMessageAction(message);
                }),
                catchError((err) => errHandler(err)),
            );
        }),
        catchError((err) => errHandler(err)),
    );

export default sendMessageEpic;
