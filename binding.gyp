{
    'targets': [
        {
            'target_name': 'dumpme',
            'sources': [
                'lib/dumpme.cc'
            ],
            'conditions': [
                ['OS=="linux"', {
                    'defines': ['TARGET_LINUX']
                }]
            ],
            'include_dirs' : [
                '<!(node -p "require(\'node-addon-api\').include_dir")'
            ],
            'defines': [
                'NAPI_VERSION=8',
                'NAPI_DISABLE_CPP_EXCEPTIONS'
            ]
        }
    ]
}
