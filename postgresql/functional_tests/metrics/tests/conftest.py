import pytest

from testsuite.databases.pgsql import discover


pytest_plugins = [
    'pytest_userver.plugins',
    'pytest_userver.plugins.samples',
    # Database related plugins
    'testsuite.databases.pgsql.pytest_plugin',
]


# /// [gate start]
@pytest.fixture(scope='session')
def pgsql_local(service_source_dir, pgsql_local_create):
    databases = discover.find_schemas(
        'pg', [service_source_dir.joinpath('schemas/postgresql')],
    )
    return pgsql_local_create(list(databases.values()))


@pytest.fixture
def client_deps(pgsql):
    pass
