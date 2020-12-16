import { connect } from 'react-redux';
import { Dispatch, bindActionCreators } from 'redux';

import { updateSudokuAction, playRoundBackwardAction } from '../../actions';

import AppBar from '../../components/AppBar';

const mapDispatchToProps = (dispatch: Dispatch) =>
    bindActionCreators(
        {
            updateSudokuAction,
            playRoundBackwardAction,
        },
        dispatch,
    );

type DispatchProps = ReturnType<typeof mapDispatchToProps>;

export type Props = DispatchProps;

export default connect(null, mapDispatchToProps)(AppBar);
