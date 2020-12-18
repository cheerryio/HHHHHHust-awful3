import { Action } from 'redux';
import { Epic } from '../';
import { addMessageAction } from '../../actions';
import { switchMap } from 'rxjs/operators';
import { EMPTY, Observable } from 'rxjs';
import { Message } from '../../types';

export const socketReceiveEpic: Epic = (action$, state$, { socket$ }) =>
    socket$.pipe(
        switchMap((socket) =>
            !socket
                ? EMPTY
                : new Observable<Action>((o) => {
                      /** 接收到消息*/
                    socket.on('receiveMessage', (message: string) => {
                          o.next(addMessageAction({ ...JSON.parse(message), isSelf: false }));
                      });

                      socket.on('id', ({ id }: { id: number }) => {
                          console.log(id);
                      });
                  }),
        ),
    );

export default socketReceiveEpic;
