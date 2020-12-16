import { connect } from 'react-redux';
import { Dispatch, bindActionCreators } from 'redux';

import {
    chooseDigitStartAction,
    updateSudokuAction,
    clearBlockHighlightAction,
    toggleDigitBoardAction,
    blockHighlightAction,
    playRoundForwardAction,
    playRoundBackwardAction,
    chooseDigitAction,
    toggleShowOptionNumberAction,
} from '../../actions';
import { Point, sudokuValue, conflictValue, PlaceValue } from '../../types';
import { StoreState } from '../../reducers';
import PlayBoard from '../../components/PlayBoard';

const mapStateToProps = ({
    Game: {
        values,
        initValues,
        point,
        digitBoard,
        blockHighlight,
        playRound,
        placeValue,
        showUnchangeable,
        conflictValues,
        showConflict,
        complete,
        showOptionNumber,
    },
}: StoreState): {
    values: sudokuValue[][];
    initValues: sudokuValue[][];
    point: Point;
    digitBoard: boolean;
    blockHighlight: number[][];
    playRound: number;
    placeValue: PlaceValue;
    showUnchangeable: boolean;
    conflictValues: conflictValue[][];
    showConflict: boolean;
    complete: boolean;
    showOptionNumber: boolean;
} => ({
    values,
    initValues,
    point,
    digitBoard,
    blockHighlight,
    playRound,
    placeValue,
    showUnchangeable,
    conflictValues,
    showConflict,
    complete,
    showOptionNumber,
});

const mapDispatchToProps = (dispatch: Dispatch) =>
    bindActionCreators(
        {
            chooseDigitStartAction,
            updateSudokuAction,
            toggleDigitBoardAction,
            blockHighlightAction,
            clearBlockHighlightAction,
            playRoundForwardAction,
            playRoundBackwardAction,
            chooseDigitAction,
            toggleShowOptionNumberAction,
        },
        dispatch,
    );

type StateProps = ReturnType<typeof mapStateToProps>;
type DispatchProps = ReturnType<typeof mapDispatchToProps>;

export type Props = StateProps & DispatchProps;

export default connect(mapStateToProps, mapDispatchToProps)(PlayBoard);
