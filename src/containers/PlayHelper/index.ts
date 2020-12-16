import { connect } from 'react-redux';
import { Dispatch, bindActionCreators } from 'redux';

import {
    blockHighlightAction,
    setPlaceValueAction,
    clearPlaceValueAction,
    chooseDigitAction,
    toggleShowUnchangeableAction,
    updateSudokuAction,
    playRoundBackwardAction,
    clearBlockHighlightAction,
    toggleShowConflictAction,
    toggleShowOptionNumberAction,
    saveGameAction,
    loadGameAction,
    setLevelAction,
} from '../../actions';
import { StoreState } from '../../reducers';

import PlayHelper from '../../components/PlayHelper';
import { PlaceValue } from '../../types';

const mapStateToProps = ({
    Game: { placeValue, complete, playRound },
}: StoreState): {
    placeValue: PlaceValue;
    complete: boolean;
    playRound: number;
} => ({
    placeValue,
    complete,
    playRound,
});

const mapDispatchToProps = (dispatch: Dispatch) =>
    bindActionCreators(
        {
            blockHighlightAction,
            setPlaceValueAction,
            clearPlaceValueAction,
            chooseDigitAction,
            toggleShowUnchangeableAction,
            updateSudokuAction,
            playRoundBackwardAction,
            clearBlockHighlightAction,
            toggleShowConflictAction,
            toggleShowOptionNumberAction,
            saveGameAction,
            loadGameAction,
            setLevelAction,
        },
        dispatch,
    );

type StateProps = ReturnType<typeof mapStateToProps>;
type DispatchProps = ReturnType<typeof mapDispatchToProps>;
export type Props = StateProps & DispatchProps;

export default connect(mapStateToProps, mapDispatchToProps)(PlayHelper);
