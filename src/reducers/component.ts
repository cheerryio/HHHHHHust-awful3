import * as actions from '../actions';
import { TOGGLE_PROGRESS } from '../actions';

type ActionType = actions.ToggleProgress;

export interface CountersStore {
    loading: boolean;
}

const init: CountersStore = {
    loading: false,
};

export default (state = init, action: ActionType): CountersStore => {
    const { loading } = state;
    switch (action.type) {
        case TOGGLE_PROGRESS:
            return { ...state, loading: !loading };
        default:
            return { ...state };
    }
};
