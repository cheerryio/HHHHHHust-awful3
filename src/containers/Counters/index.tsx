import { connect } from 'react-redux';
import { Dispatch, bindActionCreators } from 'redux';

import { incrementAction, decrementAction, getserverValueAction } from '../../actions';
import { Counters } from '../../components/Counters';
import { StoreState } from '../../reducers';

const mapStateToProps = ({ Counters: { times, serverValue } }: StoreState): { value: number; serverValue: string } => ({
    value: times,
    serverValue,
});

const mapDispatchToProps = (dispatch: Dispatch) =>
    bindActionCreators(
        {
            incrementAction,
            decrementAction,
            getserverValueAction,
        },
        dispatch,
    );

type StateProps = ReturnType<typeof mapStateToProps>;
type DispatchProps = ReturnType<typeof mapDispatchToProps>;

export type Props = StateProps & DispatchProps;

export default connect(mapStateToProps, mapDispatchToProps)(Counters);
