import QtQuick 2.6

/* Constants imports */
import "../../scripts/Constants.js" as Constants
import "../../components/" as Components

//Import C++ defined enums
import eidguiV2 1.0

PageSecurityCertificatesForm {

    Connections {
        target: gapi
        onSignalCertificatesChanged: {

            console.log("Certificates Added: "+ certificatesMap.levelCount)

            if(certificatesMap.levelCount === 5){ // Production Card
                console.log("Detected Production Card")
                propertyAcordion.model = [
                            {
                                'entity': certificatesMap.level4.OwnerName,
                                'auth': certificatesMap.level4.IssuerName,
                                'valid': certificatesMap.level4.ValidityBegin,
                                'until':certificatesMap.level4.ValidityEnd,
                                'key': certificatesMap.level4.KeyLength,
                                'status': getCertStatus(certificatesMap.level4.Status),
                                'children': [
                                    {
                                        'entity': certificatesMap.level3.OwnerName,
                                        'auth': certificatesMap.level3.IssuerName,
                                        'valid': certificatesMap.level3.ValidityBegin,
                                        'until':certificatesMap.level3.ValidityEnd,
                                        'key': certificatesMap.level3.KeyLength,
                                        'status': getCertStatus(certificatesMap.level3.Status),
                                        'children': [
                                            {
                                                'entity': certificatesMap.level2.OwnerName,
                                                'auth': certificatesMap.level2.IssuerName,
                                                'valid': certificatesMap.level2.ValidityBegin,
                                                'until':certificatesMap.level2.ValidityEnd,
                                                'key': certificatesMap.level2.KeyLength,
                                                'status': getCertStatus(certificatesMap.level2.Status),
                                                'children': [
                                                    {
                                                        'entity': certificatesMap.level1.OwnerName,
                                                        'auth': certificatesMap.level1.IssuerName,
                                                        'valid': certificatesMap.level1.ValidityBegin,
                                                        'until':certificatesMap.level1.ValidityEnd,
                                                        'key': certificatesMap.level1.KeyLength,
                                                        'status': getCertStatus(certificatesMap.level1.Status),
                                                        'children': [
                                                            {
                                                                'entity': certificatesMap.level0.OwnerName,
                                                                'auth': certificatesMap.level0.IssuerName,
                                                                'valid': certificatesMap.level0.ValidityBegin,
                                                                'until':certificatesMap.level0.ValidityEnd,
                                                                'key': certificatesMap.level0.KeyLength,
                                                                'status': getCertStatus(certificatesMap.level0.Status),
                                                            }
                                                        ]
                                                    },
                                                    {
                                                        'entity': certificatesMap.levelB1.OwnerName,
                                                        'auth': certificatesMap.levelB1.IssuerName,
                                                        'valid': certificatesMap.levelB1.ValidityBegin,
                                                        'until':certificatesMap.levelB1.ValidityEnd,
                                                        'key': certificatesMap.levelB1.KeyLength,
                                                        'status': getCertStatus(certificatesMap.levelB1.Status),
                                                        'children': [
                                                            {
                                                                'entity': certificatesMap.levelB0.OwnerName,
                                                                'auth': certificatesMap.levelB0.IssuerName,
                                                                'valid': certificatesMap.levelB0.ValidityBegin,
                                                                'until':certificatesMap.levelB0.ValidityEnd,
                                                                'key': certificatesMap.levelB0.KeyLength,
                                                                'status': getCertStatus(certificatesMap.levelB0.Status),
                                                            }
                                                        ]
                                                    }
                                                ]
                                            }
                                        ]
                                    }
                                ]
                            }
                        ]
            }else{
                console.log("test Production Card")
                propertyAcordion.model = [
                            {
                                'entity': certificatesMap.level2.OwnerName,
                                'auth': certificatesMap.level2.IssuerName,
                                'valid': certificatesMap.level2.ValidityBegin,
                                'until':certificatesMap.level2.ValidityEnd,
                                'key': certificatesMap.level2.KeyLength,
                                'status': getCertStatus(certificatesMap.level2.Status),
                                'children': [
                                    {
                                        'entity': certificatesMap.level1.OwnerName,
                                        'auth': certificatesMap.level1.IssuerName,
                                        'valid': certificatesMap.level1.ValidityBegin,
                                        'until':certificatesMap.level1.ValidityEnd,
                                        'key': certificatesMap.level1.KeyLength,
                                        'status': getCertStatus(certificatesMap.level1.Status),
                                        'children': [
                                            {
                                                'entity': certificatesMap.level0.OwnerName,
                                                'auth': certificatesMap.level0.IssuerName,
                                                'valid': certificatesMap.level0.ValidityBegin,
                                                'until':certificatesMap.level0.ValidityEnd,
                                                'key': certificatesMap.level0.KeyLength,
                                                'status': getCertStatus(certificatesMap.level0.Status),
                                            }
                                        ]
                                    },
                                    {
                                        'entity': certificatesMap.levelB1.OwnerName,
                                        'auth': certificatesMap.levelB1.IssuerName,
                                        'valid': certificatesMap.levelB1.ValidityBegin,
                                        'until':certificatesMap.levelB1.ValidityEnd,
                                        'key': certificatesMap.levelB1.KeyLength,
                                        'status': certificatesMap.levelB1.Status,
                                        'children': [
                                            {
                                                'entity': certificatesMap.levelB0.OwnerName,
                                                'auth': certificatesMap.levelB0.IssuerName,
                                                'valid': certificatesMap.levelB0.ValidityBegin,
                                                'until':certificatesMap.levelB0.ValidityEnd,
                                                'key': certificatesMap.levelB0.KeyLength,
                                                'status': getCertStatus(certificatesMap.levelB0.Status),
                                            }
                                        ]
                                    }
                                ]
                            }
                        ]
            }

            // Init Date Field with the model first option
            propertyTextEntity.propertyDateField.text = propertyAcordion.model[0].entity
            propertyTextAuth.propertyDateField.text = propertyAcordion.model[0].auth
            propertyTextValid.propertyDateField.text = propertyAcordion.model[0].valid
            propertyTextUntil.propertyDateField.text = propertyAcordion.model[0].until
            propertyTextKey.propertyDateField.text = propertyAcordion.model[0].key
            propertyTextStatus.propertyDateField.text = propertyAcordion.model[0].status
            propertyBusyIndicator.running = false
        }
    }
    Component.onCompleted: {

        console.log("Page Security Certificates Completed")
        propertyBusyIndicator.running = true
        gapi.startfillCertificateList()
    }
    function getCertStatus(certStatus){

        var networkError = "Could not validate certificate. Please check your Internet connection";
        var strCertStatus

        switch(certStatus)
        {
        case Constants.PTEID_CERTIF_STATUS_REVOKED:
            strCertStatus = "Revoked";
            break;
        case Constants.PTEID_CERTIF_STATUS_SUSPENDED:
            strCertStatus = "Inactive or Suspended";
            break;
        case Constants.PTEID_CERTIF_STATUS_VALID:
            strCertStatus = "Valid";
            break;
        //TODO: Handle the network error with a different string such as "cant validate certificate status"
        case Constants.PTEID_CERTIF_STATUS_CONNECT:
            strCertStatus = networkError
            break;
        case Constants.PTEID_CERTIF_STATUS_UNKNOWN:
            strCertStatus = "Unknown";
            break;
        default:
            strCertStatus = networkError;
            break;
        }

        return strCertStatus
    }
}
