import React, { FC, memo, SVGProps } from 'react';
import { saveIcon } from '../../consts/elements';
export const SaveSvg: FC<{
    SvgProp?: SVGProps<SVGSVGElement>;
    GProp?: SVGProps<SVGGElement>;
}> = memo(({ SvgProp = {}, GProp = {} }) => (
    <svg viewBox='0 0 24 24' {...SvgProp}>
        <g {...GProp}>{saveIcon}</g>
    </svg>
));

export default SaveSvg;
