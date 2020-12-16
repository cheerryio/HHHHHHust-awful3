import { Point, sudokuValue, PlayHistory, PlaceValue, Message, Level } from '../types';
import createActionFunc from './utils';

/** 让counter数字加一*/
export const INCREMENT = 'INCREMENT';
export type INCREMENT = typeof INCREMENT; // eslint-disable-line
export interface Increment {
    type: INCREMENT;
}
export const incrementAction = createActionFunc(INCREMENT);

/** 让counter数字减一*/
export const DECREMENT = 'DECREMENT';
export type DECREMENT = typeof DECREMENT; // eslint-disable-line
export interface Decrement {
    type: DECREMENT;
}
export const decrementAction = createActionFunc(DECREMENT);

/** get serverValue action start, gonna be handled by epic*/
export const GET_SERVERVALUE = 'GET_SERVERVALUE';
export type GET_SERVERVALUE = typeof GET_SERVERVALUE; // eslint-disable-line
export interface GetserverValue {
    type: GET_SERVERVALUE;
    payload: string;
}
export const getserverValueAction = createActionFunc(GET_SERVERVALUE, 'payload');

/** get serverValue action fullfilled, gonna be handled by counters's reducer*/
export const SERVERVALUE_FULFILLED = 'SERVERVALUE_FULFILLED';
export type SERVERVALUE_FULFILLED = typeof SERVERVALUE_FULFILLED; // eslint-disable-line
export interface ServerValueFulfilled {
    type: SERVERVALUE_FULFILLED;
    key: string;
    message: string;
}
export const serverValueFulfilled = createActionFunc(SERVERVALUE_FULFILLED, 'key', 'value');

/** 当鼠标hover上一个numberBlock，将此block的Point更新到Store中*/
export const CHOOSE_DIGIT_START = 'CHOOSE_ACTION_START';
export type CHOOSE_DIGIT_START = typeof CHOOSE_DIGIT_START; // eslint-disable-line
export interface ChooseDigitStartAction {
    type: CHOOSE_DIGIT_START;
    point: Point;
}
export const chooseDigitStartAction = createActionFunc(CHOOSE_DIGIT_START, 'point');

/** DigitBoard click a number, the clicked number will update the number in the block*/
export const CHOOSE_DIGIT = 'CHOOSE_DIGIT';
export type CHOOSE_DIGIT = typeof CHOOSE_DIGIT; // eslint-disable-line
export interface ChooseDigitAction {
    type: CHOOSE_DIGIT;
    point: Point;
}
export const chooseDigitAction = createActionFunc(CHOOSE_DIGIT, 'point');

/** 调用generateSudoku函数，生成新的数独9x9 matrix,并放入store中*/
export const UPDATE_SUDOKU = 'UPDATE_SUDOKU';
export type UPDATE_SUDOKU = typeof UPDATE_SUDOKU; // eslint-disable-line
export interface UpdateSudokuAction {
    type: UPDATE_SUDOKU;
}
export const updateSudokuAction = createActionFunc(UPDATE_SUDOKU);

/** 根据value计算新的blockHighlight矩阵*/
export const BLOCK_HIGHLIGHT = 'BLOCK_HIGHLIGHT';
export type BLOCK_HIGHLIGHT = typeof BLOCK_HIGHLIGHT; // eslint-disable-line
export interface BlockHighlightAction {
    type: BLOCK_HIGHLIGHT;
    value: sudokuValue;
}
export const blockHighlightAction = createActionFunc(BLOCK_HIGHLIGHT, 'value');

/** 将blockHighlight矩阵全部置0，没有任何一个块需要高亮*/
export const CLEAR_BLOCK_HIGHLIGHT = 'CLEAR_BLOCK_HIGHLIGHT';
export type CLEAR_BLOCK_HIGHLIGHT = typeof CLEAR_BLOCK_HIGHLIGHT; // eslint-disable-line
export interface ClearBlockHighlightAction {
    type: CLEAR_BLOCK_HIGHLIGHT;
}
export const clearBlockHighlightAction = createActionFunc(CLEAR_BLOCK_HIGHLIGHT);

/** toggle global digitBoard on and off*/
export const TOGGLE_DIGITBOARD = 'TOGGLE_DIGITBOARD';
export type TOGGLE_DIGITBOARD = typeof TOGGLE_DIGITBOARD; // eslint-disable-line
export interface ToggleDigitBoardAction {
    type: TOGGLE_DIGITBOARD;
}
export const toggleDigitBoardAction = createActionFunc(TOGGLE_DIGITBOARD);

/** playround go ahead*/
export const PLAY_ROUND_FORWARD = 'PLAY_ROUND_FORWARD';
export type PLAY_ROUND_FORWARD = typeof PLAY_ROUND_FORWARD; // eslint-disable-line
export interface PlayRoundForwardAction {
    type: PLAY_ROUND_FORWARD;
    payload: PlayHistory;
}
export const playRoundForwardAction = createActionFunc(PLAY_ROUND_FORWARD, 'payload');

/** playround go back*/
export const PLAY_ROUND_BACKWARD = 'PLAY_ROUND_BACKWARD';
export type PLAY_ROUND_BACKWARD = typeof PLAY_ROUND_BACKWARD; // eslint-disable-line
export interface PlayRoundBackwardAction {
    type: PLAY_ROUND_BACKWARD;
}
export const playRoundBackwardAction = createActionFunc(PLAY_ROUND_BACKWARD);

/** 设置placeValue值*/
export const SET_PLACE_VALUE = 'SET_PLACE_VALUE';
export type SET_PLACE_VALUE = typeof SET_PLACE_VALUE; // eslint-disable-line
export interface SetPlaceValueAction {
    type: SET_PLACE_VALUE;
    value: PlaceValue;
}
export const setPlaceValueAction = createActionFunc(SET_PLACE_VALUE, 'value');

/** 清空placeValue值，设置为null*/
export const CLEAR_PLACE_VALUE = 'CLEAR_PLACE_VALUE';
export type CLEAR_PLACE_VALUE = typeof CLEAR_PLACE_VALUE; // eslint-disable-line
export interface ClearPlaceValueAction {
    type: CLEAR_PLACE_VALUE;
    value: PlaceValue;
}
export const clearPlaceValueAction = createActionFunc(CLEAR_PLACE_VALUE, 'value');

/** 如果需要显示哪些是初始数据不需要改变，showUnchangeable=true,否则为false*/
export const TOGGLE_SHOW_UNCHANGEABLE = 'TOGGLE_SHOW_UNCHANGEABLE';
export type TOGGLE_SHOW_UNCHANGEABLE = typeof TOGGLE_SHOW_UNCHANGEABLE; // eslint-disable-line
export interface ToggleShowUnchangeableAction {
    type: TOGGLE_SHOW_UNCHANGEABLE;
}
export const toggleShowUnchangeableAction = createActionFunc(TOGGLE_SHOW_UNCHANGEABLE);

/** 如果需要显示哪些是冲突数字，showConflict=true,否则为false*/
export const TOGGLE_SHOW_CONFLICT = 'TOGGLE_SHOW_CONFLICT';
export type TOGGLE_SHOW_CONFLICT = typeof TOGGLE_SHOW_CONFLICT; // eslint-disable-line
export interface ToggleShowConflictAction {
    type: TOGGLE_SHOW_CONFLICT;
}
export const toggleShowConflictAction = createActionFunc(TOGGLE_SHOW_CONFLICT);

/** 控制是否展示框内可选数字*/
export const TOGGLE_SHOW_OPTIONNUMBER = 'TOGGLE_SHOW_OPTIONNUMBER';
export type TOGGLE_SHOW_OPTIONNUMBER = typeof TOGGLE_SHOW_OPTIONNUMBER; // eslint-disable-line
export interface ToggleShowOptionNumber {
    type: TOGGLE_SHOW_OPTIONNUMBER;
}
export const toggleShowOptionNumberAction = createActionFunc(TOGGLE_SHOW_OPTIONNUMBER);

/** 记录多次游戏的所有数据*/
export const SAVE_GAME = 'SAVE_GAME';
export type SAVE_GAME = typeof SAVE_GAME; // eslint-disable-line
export interface SaveGame {
    type: SAVE_GAME;
}
export const saveGameAction = createActionFunc(SAVE_GAME);

/** 记录多次游戏的所有数据*/
export const LOAD_GAME = 'LOAD_GAME';
export type LOAD_GAME = typeof LOAD_GAME; // eslint-disable-line
export interface LoadGame {
    type: LOAD_GAME;
    values: sudokuValue[][];
    initValues: sudokuValue[][];
    playHistorys: PlayHistory[];
    playRound: number;
}
export const loadGameAction = createActionFunc(LOAD_GAME, 'values', 'initValues', 'playHistorys', 'playRound');

/** 在聊天框发送消息*/
export const SEND_MESSAGE = 'SEND_MESSAGE';
export type SEND_MESSAGE = typeof SEND_MESSAGE; // eslint-disable-line
export interface SendMessage {
    type: SEND_MESSAGE;
    message: Message;
}
export const sendMessageAction = createActionFunc(SEND_MESSAGE, 'message');

/** 启动socket连接*/
export const START_SOCKET = 'START_SOCKET';
export type START_SOCKET = typeof START_SOCKET; // eslint-disable-line
export interface StartSocket {
    type: START_SOCKET;
}
export const startSocketAction = createActionFunc(START_SOCKET);

/** 添加一条信息到面板*/
export const ADD_MESSAGE = 'ADD_MESSAGE';
export type ADD_MESSAGE = typeof ADD_MESSAGE; // eslint-disable-line
export interface AddMessage {
    type: ADD_MESSAGE;
    message: Message;
}
export const addMessageAction = createActionFunc(ADD_MESSAGE, 'message');

/**设置游戏难度，在Store中更新level值 */
export const SET_LEVEL = 'SET_LEVEL';
export type SET_LEVEL = typeof SET_LEVEL; // eslint-disable-line
export interface SetLevel {
    type: SET_LEVEL;
    level: Level;
}
export const setLevelAction = createActionFunc(SET_LEVEL, 'level');

/**拉起进度条 */
export const TOGGLE_PROGRESS = 'TOGGLE_PROGRESS';
export type TOGGLE_PROGRESS = typeof TOGGLE_PROGRESS; // eslint-disable-line
export interface ToggleProgress {
    type: TOGGLE_PROGRESS;
}
export const toggleProgress = createActionFunc(TOGGLE_PROGRESS);
