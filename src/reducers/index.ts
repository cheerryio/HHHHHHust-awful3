import { combineReducers } from 'redux';
import CountersReducer, { CountersStore } from './counters';
import UserReducer, { UserStore } from './user';
import GameReducer, { GameStore } from './game';

export interface StoreState {
    Counters: CountersStore;
    Game: GameStore;
    User: UserStore;
}

export const reducers = combineReducers({
    Counters: CountersReducer,
    Game: GameReducer,
    User: UserReducer,
});
