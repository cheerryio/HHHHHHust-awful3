import * as actions from '../actions';
import { Message, Userinfo } from '../types';

type ActionType = actions.SendMessage;

export interface UserStore {
    messages: Message[];
    info: Userinfo;
    loading: boolean;
}

const init: UserStore = {
    messages: [],
    info: {
        username: 'cherro',
        avatar: '',
    } as Userinfo,
    loading: false,
};

const insert = (item: Message, arr: Message[]) => {
    const length = arr.length;
    if (length === 0) {
        return [item];
    }
    for (let i = length - 1; i >= 0; i--) {
        if (arr[i].time > item.time) {
            arr[i + 1] = arr[i];
        } else {
            arr[i + 1] = item;
            break;
        }
    }
    return arr.slice(-100); // just have the last n messages
};

export default (state = init, action: ActionType): UserStore => {
    switch (action.type) {
        case actions.SEND_MESSAGE:
            const messages = insert(action.message, [...state.messages]);
            return { ...state, messages };
        default:
            return { ...state };
    }
};
