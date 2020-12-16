import socketConnectEpic from './socketConnect';
import socketReceiveEpic from './socketReceive';

const epics = [socketConnectEpic, socketReceiveEpic];

export default epics;
