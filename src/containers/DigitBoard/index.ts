import { connect } from 'react-redux';
import { Dispatch, bindActionCreators } from 'redux';

import { chooseDigitAction, blockHighlightAction, playRoundForwardAction, toggleDigitBoardAction } from '../../actions';
import { Point } from '../../types';
import { StoreState } from '../../reducers';

import DigitBoard from '../../components/DigitBoard';

const mapStateToProps = ({ Game: { point, digitBoard } }: StoreState): { point: Point; digitBoard: boolean } => ({
    point,
    digitBoard,
});

const mapDispatchToProps = (dispatch: Dispatch) =>
    bindActionCreators(
        {
            chooseDigitAction,
            blockHighlightAction,
            playRoundForwardAction,
            toggleDigitBoardAction,
        },
        dispatch,
    );

type StateProps = ReturnType<typeof mapStateToProps>;
type DispatchProps = ReturnType<typeof mapDispatchToProps>;
export type Props = StateProps & DispatchProps;

export default connect(mapStateToProps, mapDispatchToProps)(DigitBoard);
