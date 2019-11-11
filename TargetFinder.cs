using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TargetFinder : MonoBehaviour
{
    [SerializeField]
    float hideDistance;

    public GameObject closetArea = null;

    void Update()
    {     
        if (closetArea != null)
        {   
            // Find direction of enemy
            var dir = closetArea.transform.position - transform.position;
            if (dir.magnitude < hideDistance)//If distance is less than hide distance disable indicator
            {
                setChildrenActive(false);// Set children false
            }
            else
            {
                setChildrenActive(true);// Set children true
                                        //Find angle between player and enemy
                var angle = Mathf.Atan2(dir.x, dir.z) * Mathf.Rad2Deg;
                transform.rotation = Quaternion.AngleAxis(angle, Vector3.up);// Rotate arrow to face enemy
            }
        }
    }
    void setChildrenActive (bool value)
    {
        foreach(Transform child in transform)// Run through all children attached
        {
            child.gameObject.SetActive(value);//Set children active or deactive depending on bool passed
        }
    }
}
