import * as actions from '../actions';

type ActionType = actions.Increment | actions.Decrement | actions.GetserverValue | actions.ServerValueFulfilled;

export interface CountersStore {
    times: number;
    serverValue: string;
}

const init: CountersStore = {
    times: 0,
    serverValue: 'init serverValue',
};

export default (state = init, action: ActionType): CountersStore => {
    const {} = state;
    switch (action.type) {
        case actions.INCREMENT:
            return { ...state, times: state.times + 1 };
        case actions.DECREMENT:
            return { ...state, times: state.times - 1 };
        case actions.SERVERVALUE_FULFILLED:
            return { ...state, serverValue: action.key };
        default:
            return { ...state, times: state.times };
    }
};
