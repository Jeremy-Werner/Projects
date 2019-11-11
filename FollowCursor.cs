using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FollowCursor : MonoBehaviour
{
    [SerializeField]
    float speed = 5f;
    // Update is called once per frame

    private bool isPaused = false;

    void Update()
    {
        if (!isPaused)
        {
            Vector3 direction = Camera.main.ScreenToWorldPoint(Input.mousePosition) - transform.position;
            var angle = Mathf.Atan2(direction.x, direction.z) * Mathf.Rad2Deg;
            Quaternion rotation = Quaternion.AngleAxis(angle, Vector3.up);
            transform.rotation = Quaternion.Slerp(transform.rotation, rotation, speed * Time.deltaTime);
        }
    }


    void OnEnable()
    {
        GameManager.onClickedPause += pause;
    }
    void OnDisable()
    {
        GameManager.onClickedPause -= pause;
    }

    void pause()
    {
        if (!isPaused)
        {
            isPaused = true;
        }
        else isPaused = false;
    }

}
