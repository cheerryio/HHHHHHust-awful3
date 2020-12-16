import * as actions from '../actions';
import { PlaceValue, Point } from '../types';
import { sudokuValue, PlayHistory, Level, conflictValue } from '../types';
import { zero9x9, null9x9 } from '../consts';
import calculateHighlight from '../algrithm/calculateHighlight';
import generateSudoku from '../algrithm/generateSudoku';
import conflictDetect from '../algrithm/conflictDetect';

type ActionType =
    | actions.ChooseDigitAction
    | actions.ChooseDigitStartAction
    | actions.UpdateSudokuAction
    | actions.BlockHighlightAction
    | actions.ClearBlockHighlightAction
    | actions.ToggleDigitBoardAction
    | actions.PlayRoundBackwardAction
    | actions.PlayRoundForwardAction
    | actions.SetPlaceValueAction
    | actions.ClearPlaceValueAction
    | actions.ToggleShowUnchangeableAction
    | actions.ToggleShowConflictAction
    | actions.ToggleShowOptionNumber
    | actions.SaveGame
    | actions.LoadGame
    | actions.SetLevel;

export interface GameStore {
    values: sudokuValue[][] /** 数独数字 9x9 matrix*/;
    initValues: sudokuValue[][] /** 数独初始化数据，标记不可变点*/;
    blockHighlight: number[][] /** 9x9 matrix ,值 1-需要高亮 ，值 0-不高亮*/;
    level: Level /** 数独难度*/;
    point: Point /** 记录当前正在操作的点*/;
    highlightPoint: Point /** 根据highlightPoing.value计算values中与该值相同的点，计算highlight矩阵*/;
    digitBoard: boolean /** 是否拉起digitBoard填数字 true-拉起 false-隐藏*/;
    playRound: number /** 记录用户下的第几个回合*/;
    playHistorys: PlayHistory[] /** 记录每一次下棋的位置（点），以及点改变之前和改变之后的值*/;
    placeValue: PlaceValue /** 如果placeValue=1到9，点击数独板上block会直接对数独板赋值数字*/;
    showUnchangeable: boolean /** 是否高亮显示数独盘上不可变数字，true-高亮显示，false-不高亮显示*/;
    conflictValues: conflictValue[][] /** 数独上冲突的点，取值null 1-9，null表示无冲突，1-9表示该位置该数字冲突*/;
    showConflict: boolean /** 是否高亮显示冲突数字*/;
    complete: boolean /** 数独是否求解成功*/;
    showOptionNumber: boolean /** 是否显示所有框里面的可选数字 true-显示 false-不显示*/;
}

const generate = generateSudoku(1)[0];
const init: GameStore = {
    values: generate,
    initValues: generate.map((x) => [...x] as sudokuValue[]),
    blockHighlight: zero9x9.map((x) => Object.assign({}, x)) as number[][],
    level: 1,
    point: { x: 0, y: 0, value: null },
    highlightPoint: { x: 0, y: 0, value: null },
    digitBoard: false,
    playRound: 0,
    playHistorys: [],
    placeValue: null,
    showUnchangeable: true,
    conflictValues: null9x9.map((x) => [...x] as conflictValue[]),
    showConflict: true,
    complete: false,
    showOptionNumber: true,
};

export default (state = init, action: ActionType): GameStore => {
    const {
        values,
        level,
        digitBoard,
        playRound,
        playHistorys,
        showUnchangeable,
        showConflict,
        showOptionNumber,
    } = state;
    switch (action.type) {
        case actions.UPDATE_SUDOKU: {
            // when click fresh button, generate sudoku and update 9x9 matrix in store
            const [generate, result] = generateSudoku(level); // eslint-disable-line
            return {
                ...init,
                values: generate,
                initValues: generate.map((x) => Object.assign({}, x)),
                blockHighlight: zero9x9.map((x) => Object.assign({}, x)) as number[][],
                conflictValues: null9x9.map((x) => [...x] as conflictValue[]),
                level,
                showUnchangeable,
                showConflict,
                showOptionNumber,
            };
        }
        case actions.BLOCK_HIGHLIGHT: // calculate highlight matrix
            return { ...state, blockHighlight: calculateHighlight(values, action.value) };
        case actions.CLEAR_BLOCK_HIGHLIGHT:
            const clear = zero9x9.map((x) => Object.assign({}, x));
            return { ...state, blockHighlight: clear };
        case actions.TOGGLE_DIGITBOARD: // show global digitBoard
            return { ...state, digitBoard: !digitBoard };
        case actions.CHOOSE_DIGIT_START: // just for update point and highlight point mouse is howvering on
            return { ...state, point: action.point };
        case actions.CHOOSE_DIGIT:
            values[action.point.x][action.point.y] = action.point.value;
            const { complete, conflictValues } = conflictDetect(values);
            return { ...state, values: values, point: action.point, conflictValues, complete };
        case actions.PLAY_ROUND_BACKWARD: {
            if (playRound === 0) return { ...state };
            if (playRound > 0)
                values[playHistorys[playRound - 1].x][playHistorys[playRound - 1].y] = playHistorys[playRound - 1].from;
            const { conflictValues } = conflictDetect(values);
            return {
                ...state,
                values: values,
                playRound: playRound - 1,
                playHistorys: playHistorys.slice(0, playRound - 1),
                conflictValues,
            };
        }
        case actions.PLAY_ROUND_FORWARD:
            playHistorys.push(action.payload);
            return {
                ...state,
                playRound: playRound + 1,
                playHistorys,
                blockHighlight: calculateHighlight(values, action.payload.to),
            };
        case actions.SET_PLACE_VALUE:
            return { ...state, placeValue: action.value };
        case actions.CLEAR_PLACE_VALUE:
            return { ...state, placeValue: null };
        case actions.TOGGLE_SHOW_UNCHANGEABLE:
            return { ...state, showUnchangeable: !showUnchangeable };
        case actions.TOGGLE_SHOW_CONFLICT:
            return { ...state, showConflict: !showConflict };
        case actions.TOGGLE_SHOW_OPTIONNUMBER:
            return { ...state, showOptionNumber: !showOptionNumber };
        case actions.LOAD_GAME: {
            const { conflictValues } = conflictDetect(action.values);
            return {
                ...state,
                values: action.values,
                initValues: action.initValues,
                playHistorys: action.playHistorys,
                playRound: action.playRound,
                conflictValues,
            };
        }
        case actions.SET_LEVEL:
            return { ...state, level: action.level };
        default:
            return { ...state };
    }
};
