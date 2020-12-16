import { connect } from 'react-redux';
import { Dispatch, bindActionCreators } from 'redux';

import { saveGameAction, loadGameAction } from '../../actions';
import { StoreState } from '../../reducers';

import HotKeys from '../../components/HotKeys';
import { PlayHistory, sudokuValue } from '../../types';

const mapStateToProps = ({
    Game: { values, initValues, playHistorys, playRound },
}: StoreState): {
    values: sudokuValue[][];
    initValues: sudokuValue[][];
    playHistorys: PlayHistory[];
    playRound: number;
} => ({
    values,
    initValues,
    playHistorys,
    playRound,
});

const mapDispatchToProps = (dispatch: Dispatch) =>
    bindActionCreators(
        {
            saveGameAction,
            loadGameAction,
        },
        dispatch,
    );

type StateProps = ReturnType<typeof mapStateToProps>;
type DispatchProps = ReturnType<typeof mapDispatchToProps>;
export type Props = StateProps & DispatchProps;

export default connect(mapStateToProps, mapDispatchToProps)(HotKeys);
