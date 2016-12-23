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
                '<!(node -e \'require("nan")\')'
            ]
        }
    ]
}
