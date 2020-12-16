import { connect } from 'react-redux';
import { Dispatch, bindActionCreators } from 'redux';

import {} from '../../actions';
import { StoreState } from '../../reducers';

import Frame from '../../components/Frame';
import { Userinfo } from '../../types';

const mapStateToProps = ({
    User: { info, loading },
}: StoreState): {
    userInfo: Userinfo;
    loading: boolean;
} => ({
    userInfo: info,
    loading,
});

const mapDispatchToProps = (dispatch: Dispatch) => bindActionCreators({}, dispatch);

type StateProps = ReturnType<typeof mapStateToProps>;
type DispatchProps = ReturnType<typeof mapDispatchToProps>;
export type Props = StateProps & DispatchProps;

export default connect(mapStateToProps, mapDispatchToProps)(Frame);
