import { Action, AnyAction } from 'redux';
import { ofType } from 'redux-observable';
import { of } from 'rxjs';
import { ajax } from 'rxjs/ajax';
import { mergeMap } from 'rxjs/operators';

import { GetserverValue, GET_SERVERVALUE, serverValueFulfilled } from '../../actions';
import { Epic } from '../';
import { API } from '../../config/consts';

export const getserverValueEpic: Epic<GetserverValue> = (action$, state$, {}) =>
    action$.pipe(
        ofType<Action, GetserverValue>(GET_SERVERVALUE),
        mergeMap(() =>
            ajax.getJSON<{ type: string; message: string }>(`${API}/user/qrCode`).pipe(
                mergeMap((res) => {
                    if (res.type === 'success') {
                        return of(serverValueFulfilled(res.type, res.message));
                    }
                    throw Error(res.message);
                }),
            ),
        ),
    );

export default getserverValueEpic;
