import openravepy
import openrave_globalstatesaver

import pytest

import pathlib
import os
import tempfile

@pytest.fixture
def tmpdir():
    with tempfile.TemporaryDirectory() as tmpdir:
        yield tmpdir

def test_datadirsaver(tmpdir):
    dir = os.path.dirname(os.path.realpath(__file__))
    ikfastrobotsdir = os.path.join(os.path.dirname(dir), 'openrave/test/ikfastrobots')

    try:
        try:
            env1 = openravepy.Environment()
            assert not env1.Load(os.path.join('fail1.dae'))

            with openrave_globalstatesaver.DataDirSaver(ikfastrobotsdir):
                env2 = openravepy.Environment()
                try:
                    assert env2.Load(os.path.join('fail1.dae'))
                finally:
                    env2.Destroy()
        finally:
            env1.Destroy()
    finally:
        openravepy.RaveDestroy()
        os.environ.pop('OPENRAVE_DATA', None)

    try:
        os.environ['OPENRAVE_DATA'] = ikfastrobotsdir
        env1 = openravepy.Environment()
        try:
            assert env1.Load(os.path.join('fail1.dae'))

            with openrave_globalstatesaver.DataDirSaver():
                env2 = openravepy.Environment()
                try:
                    assert not env2.Load(os.path.join('fail1.dae'))
                    with openrave_globalstatesaver.DataDirSaver(ikfastrobotsdir):
                        env3 = openravepy.Environment()
                        try:
                            assert env3.Load(os.path.join('fail1.dae'))
                        finally:
                            env3.Destroy()
                finally:
                    env2.Destroy()

                env2 = openravepy.Environment()
                try:
                    assert not env2.Load(os.path.join('fail1.dae')), 'OPENRAVE_DATA was not cleared by statesaver'
                finally:
                    env2.Destroy()
            assert len(env1.GetRobots()) > 0, 'exiting env1 dead after statesaver'
        finally:
            env1.Destroy()

        env1 = openravepy.Environment()
        try:
            assert env1.Load(os.path.join('fail1.dae')), 'OPENRAVE_DATA was not recovered by statesaver'
        finally:
            env1.Destroy()
    finally:
        openravepy.RaveDestroy()
        os.environ.pop('OPENRAVE_DATA', None)

def test_globalstatesaver(tmpdir):
    dir = os.path.dirname(os.path.realpath(__file__))
    ikfastrobotsdir = os.path.join(os.path.dirname(dir), 'openrave/test/ikfastrobots')

    try:
        try:
            env1 = openravepy.Environment()
            assert not env1.Load(os.path.join('fail1.dae'))

            os.environ['OPENRAVE_DATA'] = ikfastrobotsdir
            with openrave_globalstatesaver.GlobalStateSaver():
                env2 = openravepy.Environment()
                try:
                    assert env2.Load(os.path.join('fail1.dae'))
                finally:
                    env2.Destroy()
        finally:
            env1.Destroy()
    finally:
        openravepy.RaveDestroy()
        os.environ.pop('OPENRAVE_DATA', None)

    try:
        try:
            os.environ['OPENRAVE_DATA'] = ikfastrobotsdir
            env1 = openravepy.Environment()
            assert env1.Load(os.path.join('fail1.dae'))

            os.environ.pop('OPENRAVE_DATA', None)
            with openrave_globalstatesaver.GlobalStateSaver():
                env2 = openravepy.Environment()
                try:
                    assert not env2.Load(os.path.join('fail1.dae'))

                    os.environ['OPENRAVE_DATA'] = ikfastrobotsdir
                    with openrave_globalstatesaver.GlobalStateSaver():
                        env3 = openravepy.Environment()
                        try:
                            assert env3.Load(os.path.join('fail1.dae'))
                        finally:
                            env3.Destroy()
                finally:
                    env2.Destroy()

                env2 = openravepy.Environment()
                try:
                    assert not env2.Load(os.path.join('fail1.dae')), 'OPENRAVE_DATA was not cleared by statesaver'
                finally:
                    env2.Destroy()
            assert len(env1.GetRobots()) > 0, 'exiting env1 dead after statesaver'
        finally:
            env1.Destroy()

        env1 = openravepy.Environment()
        try:
            assert env1.Load(os.path.join('fail1.dae')), 'OPENRAVE_DATA was not recovered by statesaver'
        finally:
            env1.Destroy()
    finally:
        openravepy.RaveDestroy()
        os.environ.pop('OPENRAVE_DATA', None)
