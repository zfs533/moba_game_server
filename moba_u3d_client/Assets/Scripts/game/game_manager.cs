using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class game_manager :UnitySingletom<game_manager>
{
    // Start is called before the first frame update
    void Start()
    {
        event_manager.Instance.init();
        user_login.Instance.init();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
