import numpy as np
from typing import List, Union


def pageRank(P: Union[np.ndarray, List[List[int]]], d: float = .85, maxerr: float = 1e-8) -> np.ndarray:
    P = np.asarray(P, dtype=np.float)
    N: int = P.shape[0]
    linkSums: np.ndarray = P.sum(axis=0)
    for m in range(N):
        linksum = linkSums[m]
        if linksum == 0:
            continue
        P[:, m] /= linksum

    base: np.ndarray = np.array([(1 - d) / N] * N, dtype=np.float).reshape((N, 1))
    R_last, R = np.zeros(N).reshape((N, 1)), np.array([[1 / N]] * N, dtype=np.float).reshape(N, 1)
    while np.linalg.norm(R_last - R, ord=1, axis=0) > maxerr:
        R_last = R
        R = base + d * np.dot(P, R)
        R /= R.sum(axis=0)

    return R
